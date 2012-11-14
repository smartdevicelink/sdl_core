#include "TransportManager/SDeviceInfo.hpp"

NsAppLink::NsTransportManager::SDeviceInfo::SDeviceInfo()
: mDeviceHandle()
, mDeviceType()
, mUserFriendlyName()
, mUniqueDeviceId()
{
}

NsAppLink::NsTransportManager::SDeviceInfo::SDeviceInfo(tDeviceHandle DeviceHandle, EDeviceType DeviceType, std::string UserFriendlyName, std::string UniqueDeviceId)
: mDeviceHandle(DeviceHandle)
, mDeviceType(DeviceType)
, mUserFriendlyName(UserFriendlyName)
, mUniqueDeviceId(UniqueDeviceId)
{
}

NsAppLink::NsTransportManager::SDeviceInfo::SDeviceInfo(const SDeviceInfo& other)
: mDeviceHandle(other.mDeviceHandle)
, mDeviceType(other.mDeviceType)
, mUserFriendlyName(other.mUserFriendlyName)
, mUniqueDeviceId(other.mUniqueDeviceId)
{
}

bool NsAppLink::NsTransportManager::SDeviceInfo::operator==( const SDeviceInfo& i_other ) const
{
    return ( (mDeviceHandle == i_other.mDeviceHandle)
          && (mDeviceType == i_other.mDeviceType)
          && (mUserFriendlyName == i_other.mUserFriendlyName)
          && (mUniqueDeviceId == i_other.mUniqueDeviceId));
}