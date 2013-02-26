#ifndef NSAPPLINKRPCV2_UNSUBSCRIBEBUTTON_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_UNSUBSCRIBEBUTTON_RESPONSE_INCLUDE

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
    class UnsubscribeButton_response : public NsAppLinkRPC::ALRPCResponse
    {
    public:
        UnsubscribeButton_response(const UnsubscribeButton_response& c);
        UnsubscribeButton_response(void);

        virtual ~UnsubscribeButton_response(void);

        UnsubscribeButton_response& operator =(const UnsubscribeButton_response&);

        bool checkIntegrity(void);

    private:
        friend class UnsubscribeButton_responseMarshaller;
    };
}

#endif
