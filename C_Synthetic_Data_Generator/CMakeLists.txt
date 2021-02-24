#Used on non-palmetto system

project(syntheticdatagenerator LANGUAGES CXX C)
cmake_minimum_required(VERSION 3.5.1)

# require C++ 11
set (CMAKE_CXX_STANDARD 11)

#debug 
set(CMAKE_BUILD_TYPE Debug)

# convention
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# only for Linux, not Windows
if(WIN32)
	    message(FATAL_ERROR "This CMake file only supports Linux builds!")
endif()


set(AZUREIOT_INC_FOLDER ".." "/usr/local/include/azureiot" "/usr/local/include/azureiot/inc")

include_directories(${AZUREIOT_INC_FOLDER})

find_package(OpenSSL)

# NOTE: we only ever use MQTT
add_definitions(-DUSE_HTTP)
add_definitions(-DUSE_AMQP)
add_definitions(-DUSE_MQTT)

#not sure these are necessary
add_definitions(-DSET_TRUSTED_CERT_IN_SAMPLES)
include_directories("/opt/azure-iot-sdk-c/certs")

    # find Boost
find_package(Boost REQUIRED COMPONENTS math_c99 math_c99f math_c99l math_tr1 math_tr1f math_tr1l)

    # source files

    set(data_generator_c_files
    ./Advantech_Accelerometer.cpp
    ./CharDataPoint.cpp
    ./DataPoint.cpp
    ./FloatDataPoint.cpp
    ./IntegerDataPoint.cpp
    ./MIOTY.cpp
    ./Raspberry_Pi_Accelerometer.cpp
    ./Size512.cpp
    ./Size2048.cpp
    ./Size4096.cpp
    ./Size8192.cpp
    ./Size32768.cpp
	./SizeCustom.cpp
    ./Synthetic_Data_Generator.cpp
    ./Sensor.cpp
    ./ValuesDataPoint.cpp
    /opt/azure-iot-sdk-c/certs/certs.c
)

# header files
set(data_generator_h_files
    ./Advantech_Accelerometer.h
    ./CharDataPoint.h
    ./DataPoint.h
    ./FloatDataPoint.h
    ./IntegerDataPoint.h
    ./MIOTY.h
    ./Raspberry_Pi_Accelerometer.h
    ./Sensor.h
    ./ValuesDataPoint.h
    ./pareto_module/pareto_distribution.h
    ./Size512.h
    ./Size2048.h
    ./Size4096.h
    ./Size8192.h
    ./Size32768.h
	./SizeCustom.h
)

# the executable file
add_executable(generate_data ${data_generator_c_files} ${data_generator_h_files})

target_link_libraries(generate_data
       iothub_client_mqtt_transport
       iothub_client_amqp_transport
       iothub_client_http_transport
       iothub_client
       umqtt
       uhttp
       aziotsharedutil
       parson
       uuid
       pthread
       curl
       ssl
       crypto
       dl
       OpenSSL::SSL
       m
       Boost::math_c99
       Boost::math_c99f
       Boost::math_c99l
       Boost::math_tr1
       Boost::math_tr1f
       Boost::math_tr1l
)
