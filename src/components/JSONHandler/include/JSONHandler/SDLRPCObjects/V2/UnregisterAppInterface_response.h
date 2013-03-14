#ifndef NSAPPLINKRPCV2_UNREGISTERAPPINTERFACE_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_UNREGISTERAPPINTERFACE_RESPONSE_INCLUDE

#include "JSONHandler/SDLRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    robok0der
*/

namespace NsSmartDeviceLinkRPCV2
{
    class UnregisterAppInterface_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        UnregisterAppInterface_response(const UnregisterAppInterface_response& c);
        UnregisterAppInterface_response(void);

        virtual ~UnregisterAppInterface_response(void);

        UnregisterAppInterface_response& operator =(const UnregisterAppInterface_response&);

        bool checkIntegrity(void);

    private:
        friend class UnregisterAppInterface_responseMarshaller;
    };
}

#endif
