#ifndef ONAPPUNREGISTERED_CLASS
#define ONAPPUNREGISTERED_CLASS

#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/ALRPCObjects/AppInterfaceUnregisteredReason.h"

namespace RPC2Communication
{
    class OnAppUnregistered : public RPC2Notification
    {
    public:
        OnAppUnregistered();
        ~OnAppUnregistered();

        void setAppName(const std::string & appName);
        void setReason(const AppInterfaceUnregisteredReason& reason);

        std::string getAppName(void) const;
        AppInterfaceUnregisteredReason getReason(void) const;

    private:
        std::string mAppName;
        AppInterfaceUnregisteredReason mReason;
        friend class OnAppUnregisteredMarshaller;
    };
}


#endif