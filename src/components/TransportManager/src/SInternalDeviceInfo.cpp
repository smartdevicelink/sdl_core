/**
 * \file SInternalDeviceInfo.cpp
 * \brief Class SInternalDeviceInfo.
 * Copyright (c) 2013 Ford Motor Company
 */

#include "SInternalDeviceInfo.hpp"

NsSmartDeviceLink::NsTransportManager::SInternalDeviceInfo::SInternalDeviceInfo(void):
mDeviceHandle(NsSmartDeviceLink::NsTransportManager::InvalidDeviceHandle),
mUserFriendlyName(),
mUniqueDeviceId()
{
}

NsSmartDeviceLink::NsTransportManager::SInternalDeviceInfo::SInternalDeviceInfo(NsSmartDeviceLink::NsTransportManager::tDeviceHandle DeviceHandle, const std::string & UserFriendlyName, const std::string & UniqueDeviceId):
mDeviceHandle(DeviceHandle),
mUserFriendlyName(UserFriendlyName),
mUniqueDeviceId(UniqueDeviceId)
{
}
