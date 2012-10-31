#ifndef ACTIVATEAPP_REQUEST_CLASS
#define ACTIVATEAPP_REQUEST_CLASS

#include "JSONHandler/RPC2Request.h"
#include <string>
#include <vector>

namespace RPC2Communication 
{ 
    class ActivateApp : public RPC2Request
    {
    public:
        ActivateApp();
        ~ActivateApp();

        const std::vector<std::string> & getAppName() const;
        void setAppName( const std::vector<std::string> & appNames );

    private:
        std::vector<std::string> mAppNames;
        friend class ActivateAppMarshaller;

    };
}

#endif