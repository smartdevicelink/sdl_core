#ifndef NSAPPLINKRPC_RESETGLOBALPROPERTIES_RESPONSE_INCLUDE
#define NSAPPLINKRPC_RESETGLOBALPROPERTIES_RESPONSE_INCLUDE

#include "JSONHandler/ALRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   1.2
  date      2011-05-17
  generated at  Thu Jan 24 06:36:21 2013
  source stamp  Thu Jan 24 06:35:34 2013
  author    robok0der
*/

namespace NsAppLinkRPC
{
    class ResetGlobalProperties_response : public ALRPCResponse
    {
    public:
        ResetGlobalProperties_response(const ResetGlobalProperties_response& c);
        ResetGlobalProperties_response(void);

        virtual ~ResetGlobalProperties_response(void);

        ResetGlobalProperties_response& operator =(const ResetGlobalProperties_response&);

        bool checkIntegrity(void);

    private:
        friend class ResetGlobalProperties_responseMarshaller;
    };
}

#endif
