// Defined classes
#include "Sensor.h"
#include "MIOTY.h"
#include "Size512.h"
#include "Size2048.h"
#include "Size4096.h"
#include "Size8192.h"
#include "Size32768.h"
#include "SizeCustom.h"
#include "Raspberry_Pi_Accelerometer.h"
#include "Advantech_Accelerometer.h"
#include "DataPoint.h"
#include "CharDataPoint.h"
#include "FloatDataPoint.h"
#include "IntegerDataPoint.h"
#include "ValuesDataPoint.h"

const int MIN_ARGS = 8;
const int MAX_ARGS = 22;

// Finds the index of a char array within an array of char arrays
// Returns -1 if string not found in the array
int findIndex(char **array, int length, string item) {
    for(int i = 0; i < length; i++) {
        if(strcmp(array[i], item.c_str()) == 0) return i;
    }
    return -1;
}

string isBurstable(string choice, map<string, string> *argsMap) {
	if(choice == "false" || choice == "False" || choice == "no" || choice == "No") {
        return "f";
    }
    else if(choice == "true" || choice == "True" || choice == "yes" || choice == "Yes") {
        return "t";
    }
    else {
        (*argsMap)["error"] = "yes";
    }
    return "error";
}

// check arguments input by the user
// unrecognized arguments are ignored
map<string, string> checkArgs(int argc, char** argv) {
    map<string, string> argsMap;
    argsMap["error"] = "no";
    // check too little or too many arguments
    if(argc < MIN_ARGS || argc > MAX_ARGS) {
        argsMap["error"] = "yes";
    }
    // check arguments given
    else {
        // get index of --distribution: this has to exist or we return an error
        int index = findIndex(argv, argc, "--distribution");
        if(index == -1) {
            argsMap["error"] = "yes";
            return argsMap;
        }
        else {
            argsMap["distribution"] = argv[index+1]; // ***add 1 to index to get value***
        }

        // get index of --intermessage: this has to exist or we return an error
        index = findIndex(argv, argc, "--intermessage");
        if(index == -1) {
            argsMap["error"] = "yes";
            return argsMap;
        }
        else {
            argsMap["intermessage"] = argv[index+1]; // ***add 1 to index to get value***
        }

        // get index of --intermessage: this has to exist or we return an error
        index = findIndex(argv, argc, "--time");
        if(index == -1) {
			argsMap["time"] = "-1";
            //argsMap["error"] = "yes";
            //return argsMap;
        }
        else {
            argsMap["time"] = argv[index+1]; // ***add 1 to index to get value***
        }

        // get index of --shape: if exists, then interpret at double --> dafault -1
        index = findIndex(argv, argc, "--shape");
        if(index == -1) {
			if(argsMap["distribution"] == "pareto") {
				argsMap["error"] = "yes";
				return argsMap;
			}
			argsMap["shape"] = "-1";
		}
        else argsMap["shape"] = argv[index+1]; // ***add 1 to index to get value***

		//--msgcount
		index = findIndex(argv, argc, "--msgcount");
        if(index == -1) argsMap["msgcount"] = -1;
        else argsMap["msgcount"] = argv[index+1]; // ***add 1 to index to get value***


			
        // get index of --type: if exists, then interpret at string --> dafault MIOTY
        index = findIndex(argv, argc, "--type");
        if(index == -1) argsMap["type"] = "MIOTY";
        else argsMap["type"] = argv[index+1]; // ***add 1 to index to get value***

        // get index of --size: if exists, then interpret at int --> dafault -1 (none)
        index = findIndex(argv, argc, "--size");
        if(index == -1) argsMap["size"] = "-1";
        else argsMap["size"] = argv[index+1]; // ***add 1 to index to get value***

        // get index of --batch: if exists, then interpret at integer --> dafault false (no bursting)
        index = findIndex(argv, argc, "--burst");
        if(index == -1) argsMap["burst"] = "false";
        else argsMap["burst"] = isBurstable(argv[index+1], &argsMap); // ***add 1 to index to get value***

        // get index of --batch: if exists, then interpret at integer --> dafault 1 (no batching)
        index = findIndex(argv, argc, "--batch");
        if(index == -1) argsMap["batch"] = "1";
        else argsMap["batch"] = argv[index+1]; // ***add 1 to index to get value***

        // get index of --seed: if exists, then interpret at int --> dafault 1889
        index = findIndex(argv, argc, "--seed");
        if(index == -1) argsMap["seed"] = "1889";
        else argsMap["seed"] = argv[index+1]; // ***add 1 to index to get value***

        // get index of --filename: if exists, then interpret at string --> dafault myFile
        index = findIndex(argv, argc, "--filename");
        if(index == -1) argsMap["filename"] = "myFile";
        else argsMap["filename"] = argv[index+1]; // ***add 1 to index to get value***

        // get index of --protocol: if exists, then interpret at string --> dafault mqtt
        index = findIndex(argv, argc, "--protocol");
        if(index == -1) argsMap["protocol"] = "mqtt";
        else argsMap["protocol"] = argv[index+1]; // ***add 1 to index to get value***

        argsMap["error"] = "no";
    }
    return argsMap;
}

int main(int argc, char** argv) {
    // NOTE: agrv[1] is the connection string
    map<string, string> argsMap = checkArgs(argc, argv);

    // error when finding the necessary cmd line args
    if(argsMap["error"] == "yes") {
        cout << "USAGE: ./generate_data CONNECTION_STRING --distribution <dist_name> --intermessage I --time T [options]" << endl;
        cout << "Options include [--shape Sh] [--type X] [--batch B] [--burst true/false] [--seed Se] [--filename F] [--protocol P]" << endl;
        cout << "Valid values for <dist_name> are constant and pareto." << endl;
        cout << "Valid values for --protocol are mqtt, amqp, and http." << endl;
        cout << "Intermessage time is measured in MILLISECONDS and time is measured in SECONDS." << endl;
        cout << "NOTE:  In the case of a pareto distribution, [--shape Sh] is required and --intermessage is considered the scale parameter." << endl; 
        return 1;
    }

    // Define a simulated sensor object
	Sensor *my_sensor;

    // assign the specific sensor type
    if(argsMap["type"] == "MIOTY") {
        my_sensor = new MIOTY(argsMap["distribution"], atof(argsMap["intermessage"].c_str()), atof(argsMap["shape"].c_str()), atof(argsMap["time"].c_str()), atoi(argsMap["batch"].c_str()), (argsMap["burst"] == "t"), atoi(argsMap["seed"].c_str()), argv[1], argsMap["filename"], argsMap["protocol"], atoi(argsMap["msgcount"].c_str()));
    }
    else if(argsMap["type"] == "Advantech_Accelerometer") {
        my_sensor = new Advantech_Accelerometer(argsMap["distribution"], atof(argsMap["intermessage"].c_str()), atof(argsMap["shape"].c_str()), atof(argsMap["time"].c_str()), atoi(argsMap["batch"].c_str()), (argsMap["burst"] == "t"), atoi(argsMap["seed"].c_str()), argv[1], argsMap["filename"], argsMap["protocol"],atoi(argsMap["msgcount"].c_str()));
    }
    else if(argsMap["type"] == "Raspberry_Pi_Accelerometer") {
        my_sensor = new Raspberry_Pi_Accelerometer(argsMap["distribution"], atof(argsMap["intermessage"].c_str()), atof(argsMap["shape"].c_str()), atof(argsMap["time"].c_str()), atoi(argsMap["batch"].c_str()), (argsMap["burst"] == "t"), atoi(argsMap["seed"].c_str()), argv[1], argsMap["filename"], argsMap["protocol"], atoi(argsMap["msgcount"].c_str()));
    }
	else if(argsMap["type"] == "Size512"){
		my_sensor = new Size512(argsMap["distribution"], atof(argsMap["intermessage"].c_str()), atof(argsMap["shape"].c_str()), atof(argsMap["time"].c_str()), atoi(argsMap["batch"].c_str()), (argsMap["burst"] == "t"), atoi(argsMap["seed"].c_str()), argv[1], argsMap["filename"], argsMap["protocol"], atoi(argsMap["msgcount"].c_str()));
	}	
	else if(argsMap["type"] == "Size2048"){
		my_sensor = new Size2048(argsMap["distribution"], atof(argsMap["intermessage"].c_str()), atof(argsMap["shape"].c_str()), atof(argsMap["time"].c_str()), atoi(argsMap["batch"].c_str()), (argsMap["burst"] == "t"), atoi(argsMap["seed"].c_str()), argv[1], argsMap["filename"], argsMap["protocol"], atoi(argsMap["msgcount"].c_str()));
	}	
	else if(argsMap["type"] == "Size4096"){
		my_sensor = new Size4096(argsMap["distribution"], atof(argsMap["intermessage"].c_str()), atof(argsMap["shape"].c_str()), atof(argsMap["time"].c_str()), atoi(argsMap["batch"].c_str()), (argsMap["burst"] == "t"), atoi(argsMap["seed"].c_str()), argv[1], argsMap["filename"], argsMap["protocol"], atoi(argsMap["msgcount"].c_str()));
	}
    else if(argsMap["type"] == "Size8192"){
		my_sensor = new Size8192(argsMap["distribution"], atof(argsMap["intermessage"].c_str()), atof(argsMap["shape"].c_str()), atof(argsMap["time"].c_str()), atoi(argsMap["batch"].c_str()),(argsMap["burst"] == "t"), atoi(argsMap["seed"].c_str()), argv[1], argsMap["filename"], argsMap["protocol"], atoi(argsMap["msgcount"].c_str()));
	}
	else if(argsMap["type"] == "Size32768"){
		my_sensor = new Size32768(argsMap["distribution"], atof(argsMap["intermessage"].c_str()), atof(argsMap["shape"].c_str()), atof(argsMap["time"].c_str()), atoi(argsMap["batch"].c_str()),(argsMap["burst"] == "t"), atoi(argsMap["seed"].c_str()), argv[1], argsMap["filename"], argsMap["protocol"], atoi(argsMap["msgcount"].c_str()));
	}
	else if(argsMap["type"] == "SizeCustom"){
		my_sensor = new SizeCustom(argsMap["distribution"], atof(argsMap["intermessage"].c_str()), atof(argsMap["shape"].c_str()), atof(argsMap["time"].c_str()), atoi(argsMap["batch"].c_str()),(argsMap["burst"] == "t"), atoi(argsMap["seed"].c_str()), argv[1], argsMap["filename"], argsMap["protocol"],atoi(argsMap["msgcount"].c_str()), atoi(argsMap["size"].c_str()));
	}
    else {
        cout << "Sensor type " + argsMap["type"] + " not recognized. Please check that you have specified a valid sensor type and retry." << endl;
        return 2;
    }

    // Generate the data fields for the specified Sensor type
    my_sensor->GenerateTemplateFieldTypes();

    // Generate data from the sensor
    my_sensor->GenerateData();

	delete my_sensor;    

	return 0;
}
