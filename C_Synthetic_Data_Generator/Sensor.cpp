#include "Sensor.h"

// Initialize static variables
vector<chrono::steady_clock::time_point> Sensor::sendTimes;
ofstream Sensor::latencyLog;
//int SEMAPHORE = 0;
// Sensor constructor function. Sets up distribution type, parameters, 
Sensor::Sensor(string d, double i, double _shape, double t, int buf, bool bur, int rng_seed, string conn_string, string fname, string _protocol, int _msgcount) {
    // error checking
    if(d != "constant" && d != "pareto") {
        cout << "DISTRIBUTION must be either constant, pareto, or exponential (case sensitive). Please check your input value and try again with one of those 3 distributions." << endl;
        exit(EXIT_FAILURE);
    }
    else if(i <= 0) {
        cout << "INTERMESSAGE TIME is a negative number or zero. Please check your input value and try again with a postive value." << endl;
        exit(EXIT_FAILURE);
    }
    else if(d == "pareto" && _shape <= 0) {
        cout << "SCALE for Pareto distribution is a negative number or zero. Please check your input value and try again with a postive value." << endl;
        exit(EXIT_FAILURE);
    }
    else if(buf <= 0) {
        cout << "BATCH SIZE is a negative number or zero. Please check your input value and try again with a postive value." << endl;
        exit(EXIT_FAILURE);
    }
    else if(rng_seed <= 0) {
        cout << "SEED is a negative number or zero. Please check your input value and try again with a postive value." << endl;
        exit(EXIT_FAILURE);
    }
    else if(_protocol != "mqtt" && _protocol != "amqp" && _protocol != "http") {
        cout << "PROTOCOL must be either mqtt, amqp, or http (case sensitive). Please check your input value and try again with one of those 3 protocols." << endl;
        exit(EXIT_FAILURE);
    }

    // set parameters
    distribution = d.at(0); // use first character of distribution name (i.e. c, u, p, or e)
    intermessage = i;
    shape = _shape;
    time = t;
    BUFFER_MAX = buf;
    burstable = bur;
    curBufferSize = 0;
	if (time != -1){
		TOTAL_MESSAGES = (time * 1000) / intermessage;
	}
	else{
		if(_msgcount == -1 ){
			//Neither time nor msgcount given
			cout << "A time --time or Number of message --msgcount must be given. Ending" << endl;
			exit(EXIT_FAILURE);
		}
		else{
			// Using message count
			TOTAL_MESSAGES = _msgcount;
		}
	}	
	
	SECONDS_TOTAL = time;
    // initialize counter
    sendCount = 0;
    
    // open log file
	//Make sure path is given	
    Sensor::latencyLog.open(fname + "_latencies.csv");
    Sensor::latencyLog << "Message ID,Send Time,Receive Time,Status,Error" << endl;

    // remove quotes from connection string
    string quotes = "\"";
    for(int i = 0; i < (int)quotes.length(); i++)
    {
        conn_string.erase(remove(conn_string.begin(), conn_string.end(), quotes[i]), conn_string.end());
    }

    // *** Assign IoTHub stuff here *** //
    // 1. Initialize protocol object to use 
    if(_protocol == "mqtt") protocol = MQTT_Protocol;
    //else if(_protocol == "amqp") protocol = AMQP_Protocol;
    else protocol = HTTP_Protocol;
    
	// 2. Connect to IotHub device
    device_handle = IoTHubDeviceClient_CreateFromConnectionString(conn_string.c_str(), protocol);
    if (device_handle == NULL)
    {
        cout << "Failure connecting to Iothub device. Hint: Check your connection string." << endl;
        cout << "Connection string is this: " << conn_string << endl;
        cout << "This is also sometimes triggered by having no quotes around your connection string." << endl;
        exit(EXIT_FAILURE);
    }

    // Setting the auto URL Encoder (recommended for MQTT). Please use this option unless
    // you are URL Encoding inputs yourself. ONLY valid for use with MQTT
    if(_protocol == "mqtt") {
    	bool urlEncodeOn = true;
    	IoTHubDeviceClient_SetOption(device_handle, OPTION_AUTO_URL_ENCODE_DECODE, &urlEncodeOn);
    }

	#ifdef SET_TRUSTED_CERT_IN_SAMPLES
    // Setting the Trusted Certificate. This is only necessary on system with without built in certificate stores.
    // May be able to remove this option
    IoTHubDeviceClient_SetOption(device_handle, OPTION_TRUSTED_CERT, certificates);
    #endif // SET_TRUSTED_CERT_IN_SAMPLES

    // set random seed
    srand(rng_seed);

    // Set up constant character array and use ASCII to get characters
    // This character array is used to generate random characters 
    for(int i = 0; i <= 25; i++) alpha[i] = i + 65; // uppercase
    for(int i = 26; i <= 51; i++) alpha[i] = i + 71; // lowercase
    for(int i = 52; i <= 61; i++) alpha[i] = i - 4; // numbers
}


// Sensor destructor function frees pointers to
// 1) generated data maps (<string, string>)
// 2) Iothub message handles
Sensor::~Sensor() {
    for(map<string, DataPoint*>::iterator it = myTemplate.begin(); it != myTemplate.end(); it++) {
        delete it->second;
    }
    for(vector<IotHubMessage*>::iterator it = allMessages.begin(); it != allMessages.end(); it++) {
        IoTHubMessage_Destroy((*it)->messageHandle);
		delete *it;
    }
}


// This function enqueues a set of generated data from the Sensor into the messageBuffer //
void Sensor::enqueue(map<string, string> *data) {
    messageBuffer.push_back(data);
    curBufferSize++;
}


// This dequeues a set of generated data from the messageBuffer and returns it.
// NOTE: It does not free the pointer to the map
map<string, string>* Sensor::dequeue() {
    if(messageBuffer.empty()) return NULL;
    map<string, string> *data = messageBuffer.front();
    messageBuffer.pop_front();
    curBufferSize--;
    return data;
}


// This function is called when IotHub responds to our simulated device.
// It is where we record latencies and confirmations
void Sensor::send_confirm_callback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback) {
    // determine what kind of confirmation is received and increment appropriate counter
    string resultString = MU_ENUM_TO_STRING(IOTHUB_CLIENT_CONFIRMATION_RESULT, result);
    int status = -1; 
    if(resultString.find("DESTROY") != string::npos) {
        status = MESSAGE_FAILED;
    }
    else if(resultString.find("OK") != string::npos){
        status = MESSAGE_OK;
    }
    else{
        status = MESSAGE_UNKNOWN;
    }

    // cast userContextCallback to IotHubMessage pointer to get message id
    IotHubMessage *callback = (IotHubMessage*)userContextCallback;

    // get current time in milliseconds
	std::chrono::milliseconds receiveTime = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch());

    // log the latencies and stuff
    Sensor::latencyLog << callback->id << "," 
        << chrono::duration_cast<chrono::milliseconds>(Sensor::sendTimes.at(callback->id).time_since_epoch()).count() << "," 
        << receiveTime.count() << "," 
        << status << "," 
		<< "\"" << resultString << "\"" << endl;
	
	//SEMAPHORE--;

}

int stringSize(char* s) {
    int count = 0;
    while(s[count] != '\0') count++;
    return count;
}

// This function sends data generated by the Sensor to Azure
void Sensor::SendDataToAzure(map<string, string> *genData) {
    // error checking
    if(genData == NULL) {
        cout << "WARNING: empty genData map" << endl;
        return;
    }

    // initialize a json object; this will store our sensor data
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    char *telemetry_msg = NULL;
    // fill out the telemetry messsage with the key-values in each map contained in genData
    for(map<string, string>::iterator it = genData->begin(); it != genData->end(); it++) {
        if(it->first.find(".") != string::npos) json_object_dotset_string(root_object, it->first.c_str(), it->second.c_str());
        else json_object_set_string(root_object, it->first.c_str(), it->second.c_str());
    }
    // serialize json to a string
    telemetry_msg = json_serialize_to_string_pretty(root_value);

    // create a new message to send to IoTHub and add to the allMessages vector
    allMessages.push_back(new IotHubMessage(IoTHubMessage_CreateFromString(telemetry_msg), sendCount));
	
	//SEMAPHORE++;
    // record time at which we send message
    Sensor::sendTimes.push_back(chrono::steady_clock::now());
    // send the message to IoTHub
    IoTHubDeviceClient_SendEventAsync(device_handle, allMessages.at(sendCount)->messageHandle, Sensor::send_confirm_callback, allMessages.at(sendCount));

    // increase count of sent messages
    sendCount++;

    // free allocated memory
    json_free_serialized_string(telemetry_msg);
    json_value_free(root_value);
	// free map
    delete genData;
}


// This function sets up the fields of data the Sensor can generate.
// It is implemented in subclasses to Sensor
void Sensor::GenerateTemplateFieldTypes() {
    cout << "Sensor GenerateTemplateFieldTypes" << endl;
}


// This function generates data from the sensor for...
// 1) the specified amount of time
// 2) the specified time between sending messages (intermessage time)
// 3) intermessage time according to a specified statistical distribution
void Sensor::GenerateData() {
    // initialize the IotHub SDK
    (void)IoTHub_Init();

	// seed the random number generator
    std::seed_seq seed{};
    std::default_random_engine eng;
    pareto_distribution<double> p(shape, intermessage);

    // keep track of total time generating data
    // int timeElapsed = 0;
    // start time
    // start = chrono::steady_clock::now();

	// Create a map to represent the generated data
	// for your reference, Jack, in C++ we should probably use so-called "smart pointers", but I have never used them before

	map<string, string> *genData = NULL;	


    // generate the data for specified time
//	auto start = std::chrono::steady_clock::now();
//    while( ( (std::chrono::duration<double>)(std::chrono::steady_clock::now()-start) ).count() <= time){
	while(sendCount < TOTAL_MESSAGES){
        // initialize new map (gets deleted in SendDataToAzure function)
		genData = new map<string, string>();

        // generate data from sensor based on the data points in its myTemplate
        for(map<string, DataPoint*>::iterator it = myTemplate.begin(); it != myTemplate.end(); it++) {
            string valType = it->second->getType(); // this is a DataPoint*

            // Convert DataPoint to the right type
            // 1. DataPoint is an integer value within a range
            if(valType == "int") {
                if(IntegerDataPoint *IDP = dynamic_cast<IntegerDataPoint*>(it->second)) {
                    pair<int, int> range = IDP->getRange();
                    int width = range.second - range.first + 1; // add 1 for inclusive range
                    (*genData)[it->first] = to_string(range.first + rand() % width); // e.g. [100, 200] -> 100 + random 0-100
                }
                else {
                    cout << "Error downcasting from DataPoint to IntegerDataPoint" << endl;
                    exit(EXIT_FAILURE);
                }
            }

            // 2. DataPoint is a float value within a range
            else if(valType == "float") {
                if(FloatDataPoint *FDP = dynamic_cast<FloatDataPoint*>(it->second)) {
                    pair<double, double> range = FDP->getRange();
                    double width = range.second - range.first;
                    (*genData)[it->first] = to_string(range.first + ((double)rand() / RAND_MAX) * width); // e.g. [1.5, 4.0] -> 1.5 + random 0-1 * 2.5
                }
                else {
                    cout << "Error downcasting from DataPoint to FloatDataPoint" << endl;
                    exit(EXIT_FAILURE);
                }
            }

            // 3. DataPoint is a timestamp
            else if(valType == "datetime") {
                (*genData)[it->first] = "08:00:00 AM Friday Feb 14, 2020"; // TODO implement
            }

            // 4. DataPoint is an array of random characters
            else if(valType == "char") {
                if(CharDataPoint *CDP = dynamic_cast<CharDataPoint*>(it->second)) {
                    CharacterType ct = CDP->getCharType();
                    int len = CDP->getLength();
                    char *generatedChars = new char[len+1];
                    if(ct == CharacterType::alpha) {
                        for(int i = 0; i < len; i++) {
                            generatedChars[i] = alpha[rand() % 52];
                        }
                    }
                    else if(ct == CharacterType::alphanumeric) {
                        for(int i = 0; i < len; i++) {
                            generatedChars[i] = alpha[rand() % 62];
                        }
                    }
                    else {
                        cout << "Error: unknown CharacterType";
                        exit(EXIT_FAILURE);
                    }
                    generatedChars[len] = '\0'; // NULL terminate character array
                    (*genData)[it->first] = generatedChars;
					delete [] generatedChars; // free allocated array
                }
                else {
                    cout << "Error downcasting from DataPoint to CharDataPoint" << endl;
                    exit(EXIT_FAILURE);
                }
            }

            // 5. DataPoint is randomly picked among a set of values
            else if(valType == "values") {
                if(ValuesDataPoint *VDP = dynamic_cast<ValuesDataPoint*>(it->second)) {
                    vector<string> values = VDP->getValues();
                    (*genData)[it->first] = values.at(rand() % (int)values.size());
                }
                else {
                    cout << "Error downcasting from DataPoint to ValuesDataPoint" << endl;
                    exit(EXIT_FAILURE);
                }
            }

            // 6. Error: DataPoint isn't one of the 5 types
            else {
                cout << "Unexpected DataPoint type. Terminating..." << endl;
                exit(EXIT_FAILURE);
            }
        }

        // buffer data read from sensor
        enqueue(genData);

        // send data in buffer once we reach desired batch size
        if(curBufferSize >= BUFFER_MAX) {
            map<string, string> *data = dequeue();
            while(data != NULL) {
                // send message
                SendDataToAzure(data);
                data = dequeue();
            }
        }

        // get intermessage time
        int actualInterMsg;
        // if burstable, 5% chance of bursting up to 10 messages in a row
        if(burstable && (rand() % 20) == 0) {
            burstInterval = 1 + rand() % 10;
        }

        if(distribution == 'c') actualInterMsg = (int)intermessage;
        else if(distribution == 'p') actualInterMsg = (int)p(eng);
        else {
            cout << "Error unknown distribution when calculating intermessage time" << endl;
            exit(EXIT_FAILURE);
        }

        // bursting mode
        if(burstInterval > 0) {
            actualInterMsg *= 0.5;
            burstInterval--;
        }

		
		// wait before generating data again
        this_thread::sleep_for(chrono::milliseconds(actualInterMsg));
	}
	
	//Stall program until all callbacks are received 
//	while(SEMAPHORE != 0) {
//		this_thread::sleep_for(chrono::milliseconds(100));
//	}	
	

    // Clean up the iothub sdk handle
    IoTHubDeviceClient_Destroy(device_handle);

    // Free all the sdk subsystem
    IoTHub_Deinit();

    // close latency file
    Sensor::latencyLog.close();
}
