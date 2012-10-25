#include "SInternalDeviceInfo.hpp"

NsAppLink::NsTransportManager::SInternalDeviceInfo::SInternalDeviceInfo(void):
mDeviceHandle(NsAppLink::NsTransportManager::InvalidDeviceHandle),
mUserFriendlyName()
{
}

NsAppLink::NsTransportManager::SInternalDeviceInfo::SInternalDeviceInfo(NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle, const std::string & UserFriendlyName):
mDeviceHandle(DeviceHandle),
mUserFriendlyName(UserFriendlyName)
{
}
