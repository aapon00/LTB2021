# Organization of the C++ Code
- The main driver and all associated classes are contained in this folder.
- When you compile the code, you will want to make a folder called `cmake` and build the executable there with CMake. See the main README for details on how to compile.
- The `pareto_module` folder contains a header file for generating random numbers according to a Pareto distribution. It also has an example of how to use it.
- The `Tests` folder contains a few files related to tests we ran to make sure the code was working as intended.
- The `issue` folder contains a narrowed-down version of our code. We used it when submitted an issue for the SDK involving segfaults at high frequency sending rates.
## Classes and Components
1. **Synthetic_Data_Generator** - this is the main driver. It takes in command line arguments and instantitates a Sensor object to generate data and send to Azure.
2. **Sensor** - this class represents a single sensor. It handles data generation, sending that data to Azure, and logging send & receive times to a csv file.
3. **MIOTY**, Advantech_Accelerometer, Raspberry_Pi_Accelerometer - subclasses of the Sensor class.
4. **DataPoint** - represents a single data point "read" by the Sensor.
5. **(Char/Float/Integer/Values)DataPoint** - subclasses of the DataPoint class.
    1. CharDataPoint represents a string of random characters.
    2. FloatDataPoint represents a random float value "read" from a certain range.
    3. IntegerDataPoint represents a random integer value "read" from a certain range.
    4. ValuesDataPoint represents a random value "read" from a specific set of strings.
# Cloning the Azure C IoT SDK from GitHub repo (local machine with root access)
 - **Unfortunately, I don't how Robert and Wajdi got the SDK working with Spack on Palmetto**
 - This is an important link: [https://github.com/Azure/azure-iot-sdk-c/blob/master/doc/devbox_setup.md#linux]
 - When installing the SDK via cloning the GitHub repo, you need to also init the submodules using `git submodule update --init` and proceed to use cmake to build the submodules (**which are c-utility aka macro-utils, uamqp, and umqtt**) by doing the following:
 ```
    mkdir cmake
    cd cmake
    cmake ..
    sudo cmake --build . --target install
    # And then I did the following to install the azure-iot-sdk-c:
    (from cmake folder) cmake -Duse_installed_dependencies=ON ..
    sudo cmake --build . --target install
```
