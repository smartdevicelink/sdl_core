/**
 * \file CDevice.cpp
 * \brief CDevice class implementation.
 * Copyright (c) 2013 Ford Motor Company
 */

#include "Logger.hpp"

#include "ConnectionHandler/CDevice.hpp"

/**
 * \namespace NsConnectionHandler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */ 
namespace NsConnectionHandler
{

    log4cplus::Logger CDevice::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("ConnectionHandler"));

    CDevice::CDevice(tDeviceHandle aDeviceHandle, std::string aUserFriendlyName):
    mDeviceHandle(aDeviceHandle),
    mUserFriendlyName(aUserFriendlyName)
    {

    }

    CDevice::~CDevice()
    {
        
    }

    tDeviceHandle CDevice::getDeviceHandle() const
    {
        return mDeviceHandle;
    }

    std::string CDevice::getUserFriendlyName() const
    {
        return mUserFriendlyName;
    }
}/* namespace NsConnectionHandler */
