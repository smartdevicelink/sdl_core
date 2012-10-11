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
#include "ProtocolHandler.hpp"

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

    /*** Components instance section***/
    /**********************************/
    NsTransportLayer::CBTAdapter btadapter;
    AxisCore::ProtocolHandler protocolHandler =  AxisCore::ProtocolHandler((AxisCore::IProtocolObserver*)NULL, &btadapter);
    /**********************************/

    int rfcommsock;
    int scosock;

    /*** Start BT Devices Discovery***/

    std::vector<NsTransportLayer::CBTDevice> devicesFound;
    btadapter.scanDevices(devicesFound);
    if (0 < devicesFound.size())
    {
        printf("Found %d devices\n", devicesFound.size());
        printf("Please make your choice:\n");
    } else
    {
        printf("No any devices found!\n");
        return EXIT_SUCCESS;
    }

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
        return EXIT_SUCCESS;
    }

    /*** Start SDP Discovery on device***/

    std::vector<int> portsRFCOMMFound;
    btadapter.startSDPDiscoveryOnDevice(discoveryDeviceAddr.c_str(), portsRFCOMMFound);
    if (0 < portsRFCOMMFound.size())
    {
        printf("Found %d ports on %s device\n", portsRFCOMMFound.size(), discoveryDeviceAddr.c_str());
        printf("Please make you choice:\n");
    } else
    {
        printf("No any ports discovered!\n");
        return EXIT_SUCCESS;
    }

    std::vector<int>::iterator itr;
    int j = 0;
    for(itr = portsRFCOMMFound.begin(); itr != portsRFCOMMFound.end(); itr++)
    {
        printf("%d: %d \n", j++, *itr);
    }

    std::cin >> j;
    int portRFCOMM = 0;
    if (j < portsRFCOMMFound.size())
    {
        portRFCOMM = portsRFCOMMFound[j];
    } else
    {
        printf("Bad choice!");
        return EXIT_SUCCESS;
    }

    /*** Start RFCOMM connection***/

    int sockID = btadapter.startRFCOMMConnection(discoveryDeviceAddr.c_str(), portRFCOMM);

    if (0 < sockID)
    {
        btadapter.processRFCOMM(sockID);
    }

    return EXIT_SUCCESS;
} 