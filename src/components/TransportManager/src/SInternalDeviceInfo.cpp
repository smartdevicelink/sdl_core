#include "SInternalDeviceInfo.hpp"

NsAppLink::NsTransportManager::SInternalDeviceInfo::SInternalDeviceInfo(void):
mDeviceHandle(NsAppLink::NsTransportManager::InvalidDeviceHandle),
mUserFriendlyName(),
mUniqueDeviceId()
{
}

NsAppLink::NsTransportManager::SInternalDeviceInfo::SInternalDeviceInfo(NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle, const std::string & UserFriendlyName, const std::string & UniqueDeviceId):
mDeviceHandle(DeviceHandle),
mUserFriendlyName(UserFriendlyName),
mUniqueDeviceId(UniqueDeviceId)
{
}
