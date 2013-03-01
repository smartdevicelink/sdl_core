#ifndef NSAPPLINKRPC_SUBSCRIBEBUTTON_RESPONSE_INCLUDE
#define NSAPPLINKRPC_SUBSCRIBEBUTTON_RESPONSE_INCLUDE

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
    class SubscribeButton_response : public ALRPCResponse
    {
    public:
        SubscribeButton_response(const SubscribeButton_response& c);
        SubscribeButton_response(void);

        virtual ~SubscribeButton_response(void);

        SubscribeButton_response& operator =(const SubscribeButton_response&);

        bool checkIntegrity(void);

    private:
        friend class SubscribeButton_responseMarshaller;
    };
}

#endif
