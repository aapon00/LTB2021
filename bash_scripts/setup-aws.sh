#!/bin/bash
#Sets up an instance for experiements. Can work on any Ubuntu machine with root
#run as root
#Log file when run as setup in aws is /var/log/cloud-init-output.log
#Ubuntu Server 18.04 LTS (HVM), SSD Volume Type

echo USER INPUT SETUP SCRIPT BEGIN

if (( $EUID != 0 )); then
    echo "Please run as root"
    exit 1
fi

#echos all commands to stdout
set -x
 
#Allows grub update to proceed automatically 
export DEBIAN_FRONTEND=noninteractive 

BASH=/bin/bash

IOT_SDK_V=LTS_02_2020_Ref01
CMAKE_LOC=https://github.com/Kitware/CMake/releases/download/v3.16.2/cmake-3.16.2-Linux-x86_64.sh


#update, upgrade, install gcc, ssl, and other libraries required by iot-sdk. Also boost-dev for pareto
apt update
apt upgrade -y
apt install -y build-essential libcurl4-openssl-dev libssl-dev uuid-dev libboost-all-dev


if [[ ! -d /opt ]]; then
	mkdir /opt
fi

#setup cmake. create sym link to binary
cd /opt
wget $CMAKE_LOC --output-document=cmake_3.16.2.sh
#Accept license. Install in /opt/cmake-3.16.2-Linux-x86_64
$BASH /opt/cmake_3.16.2.sh --skip-license --prefix=/opt --include-subdir
ln -s /opt/cmake-3.16.2-Linux-x86_64/bin/cmake /usr/local/bin/cmake

#If you need spack...
#download spack and setup spack
#cd /opt
#git clone https://github.com/spack/spack.git
#. /opt/spack/share/spack/setup-env.sh
#spack compiler add
#spack --version


#download and setup azure iot sdk
cd /opt
git clone -b $IOT_SDK_V https://github.com/Azure/azure-iot-sdk-c.git
cd azure-iot-sdk-c
git submodule update --init
mkdir /opt/azure-iot-sdk-c/cmake && cd /opt/azure-iot-sdk-c/cmake/
cmake .. && cmake --build . && make install
RES=$?

if [[ $RES -ne 0 ]]; then
	echo "ERROR: AZUREIOT SDK FAILED TO INSTALL"
	touch /home/ubuntu/SETUP_FAILED
else
	touch /home/ubuntu/SETUP_SUCCESS
fi

