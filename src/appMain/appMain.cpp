/**
 * \file appMain.cpp
 * \brief AppLink main application sources
 * \author AKara
 */

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>

#include "appMain.hpp"
#include "CBTAdapter.hpp"

/**
 * \brief Entry point of the program.
 * \param argc number of argument
 * \param argv array of arguments
 * \return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char** argv)
{
    unsigned int cls = 0x280404;
    int timeout = 1000;
    uint8_t channel = 3;

    NsTransportLayer::CBTAdapter btadapter;

    int rfcommsock;
    int scosock;

    std::vector<NsTransportLayer::CBTDevice> devicesFound;
    btadapter.scanDevices(devicesFound);
    printf("Found %d devices\n", devicesFound.size());
    printf("Please make you choice:\n");
    std::vector<NsTransportLayer::CBTDevice>::iterator it;
    int i = 0;
    for(it = devicesFound.begin(); it != devicesFound.end(); it++)
    {
        NsTransportLayer::CBTDevice device = *it;
        printf("%d: %s %s \n", i++, device.getDeviceAddr().c_str(), device.getDeviceName().c_str());
    }
    std::cin >> i;
    std::string discoveryDeviceAddr = "";
    if (i < devicesFound.size())
    {
        discoveryDeviceAddr = devicesFound[i].getDeviceAddr();
    } else
    {
        printf("Bad choice!");
        return 0;
    }
    btadapter.startSDPDiscoveryOnDevice(discoveryDeviceAddr.c_str());
    return 0;

    if (btadapter.setClass(cls, timeout) < 0)
    {
        perror("set_class ");
    }

    if (btadapter.registerSDP(channel) < 0)
    {
        perror("register_sdp ");
        return -1;
    }
    char targetDevID[18];
    if ((rfcommsock = btadapter.rfcommListen(channel, targetDevID)) < 0)
    {
        perror("set_class ");
        return -1;
    } else
    {
        //btadapter.processRFCOMM(rfcommsock);
        //printf("Target device ID %s \n", targetDevID);
    }

    if ((scosock = btadapter.scoListen()) < 0)
    {
        perror("sco_listen ");
        return -1;
    }

    btadapter.handleConnection(rfcommsock, scosock);

    return EXIT_SUCCESS;
} 