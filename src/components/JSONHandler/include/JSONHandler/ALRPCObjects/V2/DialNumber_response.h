#ifndef NSAPPLINKRPCV2_DIALNUMBER_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_DIALNUMBER_RESPONSE_INCLUDE

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
    class DialNumber_response : public NsAppLinkRPC::ALRPCResponse
    {
    public:
        DialNumber_response(const DialNumber_response& c);
        DialNumber_response(void);

        virtual ~DialNumber_response(void);

        DialNumber_response& operator =(const DialNumber_response&);

        bool checkIntegrity(void);

    private:
        friend class DialNumber_responseMarshaller;
    };
}

#endif
