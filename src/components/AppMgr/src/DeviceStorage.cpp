#include "AppMgr/DeviceStorage.hpp"

using namespace NsAppManager;

DeviceStorage::DeviceStorage(int deviceHandle, const std::string & deviceName) :
mDeviceHandle( deviceHandle )
, mUserFriendlyName( deviceName )
, mApplicationOnDeviceID( 0 )
{}
 
DeviceStorage::~DeviceStorage()
{}

void DeviceStorage::setAppId( int appId )
{
    mApplicationOnDeviceID = appId;
}

int DeviceStorage::getAppId() const
{
    return mApplicationOnDeviceID;
}

int DeviceStorage::getDeviceHandle() const
{
    return mDeviceHandle;
}

const std::string & DeviceStorage::getUserFriendlyName() const
{
    return mUserFriendlyName;
}

void DeviceStorage::addConnectionKey(int connectionKey)
{
    mConnectionKeys.insert(connectionKey);
}
    
bool DeviceStorage::hasConnectionKey(int connectionKey)
{
    return (mConnectionKeys.find(connectionKey) != mConnectionKeys.end());
}

void DeviceStorage::removeConnectionKey(int connectionKey)
{
    mConnectionKeys.erase(connectionKey);
}

const std::set<int> & DeviceStorage::getConnectionKeys() const
{
    return mConnectionKeys;
}