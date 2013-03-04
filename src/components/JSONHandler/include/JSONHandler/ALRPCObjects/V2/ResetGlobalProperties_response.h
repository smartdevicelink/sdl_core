#ifndef NSAPPLINKRPCV2_RESETGLOBALPROPERTIES_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_RESETGLOBALPROPERTIES_RESPONSE_INCLUDE

#include "JSONHandler/ALRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    robok0der
*/

namespace NsAppLinkRPCV2
{
    class ResetGlobalProperties_response : public NsAppLinkRPC::ALRPCResponse
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
