#ifndef NSAPPLINKRPC_SETGLOBALPROPERTIES_RESPONSE_INCLUDE
#define NSAPPLINKRPC_SETGLOBALPROPERTIES_RESPONSE_INCLUDE

#include "JSONHandler/SDLRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   1.2
  date      2011-05-17
  generated at  Thu Jan 24 06:36:21 2013
  source stamp  Thu Jan 24 06:35:34 2013
  author    robok0der
*/

namespace NsSmartDeviceLinkRPC
{
    class SetGlobalProperties_response : public SDLRPCResponse
    {
    public:
        SetGlobalProperties_response(const SetGlobalProperties_response& c);
        SetGlobalProperties_response(void);

        virtual ~SetGlobalProperties_response(void);

        SetGlobalProperties_response& operator =(const SetGlobalProperties_response&);

        bool checkIntegrity(void);

    private:
        friend class SetGlobalProperties_responseMarshaller;
    };
}

#endif
