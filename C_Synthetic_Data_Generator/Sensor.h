#ifndef SENSOR_H
#define SENSOR_H

// standard libraries
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib> // for exit() and srand and atof
#include <cstring> // for strcpy, strlen
#include <ctime> // for converting time_t to string
#include <map>
#include <vector>
#include <deque>
#include <iterator>
#include <chrono> // for measuring time
#include <thread> // for sleeping
#include <algorithm> // for std::remove

// for the pareto distribution
#include "pareto_module/pareto_distribution.h"

// our made up classes
#include "DataPoint.h"
#include "IntegerDataPoint.h"
#include "FloatDataPoint.h"
#include "CharDataPoint.h"
#include "ValuesDataPoint.h"

// Azure libraries
#include "iothub.h"
#include "iothub_device_client.h" //Change from _LL
#include "iothub_client_options.h"
#include "iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/shared_util_options.h"
#include "parson.h"

// protocols
#include "iothubtransportmqtt.h"
//#include "iothubtransportamqp.h" --> causing errors on Palmetto compilation
#include "iothubtransporthttp.h"

// for certs but can prob be removed
#ifdef SET_TRUSTED_CERT_IN_SAMPLES
    #include "certs.h"
#endif // SET_TRUSTED_CERT_IN_SAMPLES

// for ease of use...or laziness
using namespace std;


// Struct representing a message sent to iothub with a tracking id
struct IotHubMessage {
    IOTHUB_MESSAGE_HANDLE messageHandle;
    int id;
    IotHubMessage(IOTHUB_MESSAGE_HANDLE imh, int the_id) {
        messageHandle = imh;
        id = the_id;
    }
};

#define MESSAGE_FAILED 0
#define MESSAGE_OK 1
#define MESSAGE_UNKNOWN 2

class Sensor {
    private:
        // *** Parameters *** //
        char distribution; // distribution type
        double intermessage; // intermessage time MEASURED IN MILLISECONDS
        double shape; // shape parameter of the pareto distributions
        double time; // total data generation duration MEASURED IN SECONDS
        int BUFFER_MAX; // size of batch of messages to send to Azure at once
        int curBufferSize; // track current number of things in buffer
		int SECONDS_TOTAL;
	    int TOTAL_MESSAGES; // total number of messages to send
        bool burstable; // whether or not this sensor can burst
        int burstInterval; // when a burst is encountered, this is how long it will last in number of messages (set to 0 when not in burst)

        // *** Utility *** //
        int sendCount; // keep track of messages sent
        char alpha[62]; // array of alphabet and numbers
        
        // *** Vectors *** //
        vector<IotHubMessage*> allMessages;  // to store message id's and message handles on heap (timing latencies breaks otherwise, need to be on the heap)

        // *** IOTHUB stuff *** //
        IOTHUB_CLIENT_TRANSPORT_PROVIDER protocol; // can be MQTT, AQMP, or HTTP
        IOTHUB_DEVICE_CLIENT_HANDLE device_handle;

        // *** Message buffer *** //
        deque<map<string, string>* > messageBuffer;
    
    protected:
        map<string, DataPoint*> myTemplate; // fields of generated data

    public:
        // *** Methods *** //
        Sensor(string d, double i, double _shape, double t, int buf, bool bur, int rng_seed, string conn_string, string fname, string _protocol, int _msgcount);
        virtual ~Sensor();
        void enqueue(map<string, string> *data);
        map<string, string>* dequeue();
        void SendDataToAzure(map<string, string> *genData);
        virtual void GenerateTemplateFieldTypes();
        void GenerateData();

        // STATIC MEMBERS. NECESSARY EVIL FOR THE CALLBACK FUNCTION. //

        // *** Vectors *** //
        static vector<chrono::steady_clock::time_point> sendTimes; // track send times of messages

        // *** Latency log file *** //
        static ofstream latencyLog;

        // *** Static Method *** //
        static void send_confirm_callback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback);
};

#endif
