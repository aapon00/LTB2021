#!/bin/bash

#use -o none to silent and -o tsv for tab format

# NOTE: this PROVISIONS a NEW resource group and IoTHub and thens deletes them
# NOTE: used for OLD python version


location=eastus
resourceGroup=dice
SKU=$SKU 

pwd

#scripts/deploy_iot_hub.sh
CONNECT_STRING=""

    az account show


    subscriptionID=$(az account show --query id -o tsv)

    #prints already installed and returns 0. otherwises installs
    az extension add --name azure-cli-iot-ext
	
    echo "location: " $location

    # Concatenate this number onto the resources that have to be globally unique.
    # You can set this to "" or to a specific value if you don't want it  to be random.
    # This retrieves a random value.
    randomValue=$RANDOM

    # Create the resource group to be used
    #   for all the resources for this tutorial.
    az group create --name $resourceGroup --location "$location"
    az group create --name $resourceGroup --location eastus

    iotHubName=dicehub$randomValue
    iotDeviceName=python_device$randomValue
    echo "IOT hub name: " $iotHubName

    az iot hub create --name $iotHubName \
        --resource-group $resourceGroup \
        --sku $SKU --location $location

    az iot hub device-identity create --device-id $iotDeviceName \
        --hub-name $iotHubName

CONNECT_STRING=$(az iot hub device-identity show-connection-string --device-id $iotDeviceName --hub-name $iotHubName --query "connectionString" )



echo $CONNECT_STRING

#delete resourcegroup
#az group delete --name dice --yes
