#ifndef GETCAPABILITIES_RESPONSE_CLASS
#define GETCAPABILITIES_RESPONSE_CLASS

#include "JSONHandler/RPC2Response.h"
#include "JSONHandler/ALRPCObjects/ButtonCapabilities.h"
#include <vector>

namespace RPC2Communication
{
    class GetCapabilitiesResponse : public RPC2Response
    {
    public:
        struct GetCapabilitiesResponseInternal
        {
            ButtonCapabilities capability;
            std::string button;
        };
        GetCapabilitiesResponse();
        ~GetCapabilitiesResponse();

        void setCapabilities(const std::vector<GetCapabilitiesResponseInternal> &v);
        std::vector<GetCapabilitiesResponseInternal> getCapabilities() const;
    
    private:
        std::vector<GetCapabilitiesResponseInternal> mCapabilities;
        friend class GetCapabilitiesResponseMarshaller;
    };
}


#endif