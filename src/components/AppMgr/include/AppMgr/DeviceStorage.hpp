#ifndef DEVICESTORAGE_CLASS
#define DEVICESTORAGE_CLASS

#include <string>
#include <set>

namespace NsAppManager
{
    class DeviceStorage
    {
    public:
        DeviceStorage(int deviceHandle, const std::string & deviceName);
        ~DeviceStorage();

        void setAppId( int appId );
        int getAppId() const;
        int getDeviceHandle() const;
        const std::string & getUserFriendlyName() const;
        void addConnectionKey(int connectionKey);
        bool hasConnectionKey(int connectionKey);
        void removeConnectionKey(int connectionKey);
        const std::set<int> & getConnectionKeys() const;
    
    private:
        int mDeviceHandle;
        std::string mUserFriendlyName;
        int mApplicationOnDeviceID;
        std::set<int> mConnectionKeys;
    };
}

#endif // DEVICESTORAGE_CLASS