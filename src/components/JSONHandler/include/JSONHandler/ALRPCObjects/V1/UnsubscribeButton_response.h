#ifndef NSAPPLINKRPC_UNSUBSCRIBEBUTTON_RESPONSE_INCLUDE
#define NSAPPLINKRPC_UNSUBSCRIBEBUTTON_RESPONSE_INCLUDE

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
    class UnsubscribeButton_response : public ALRPCResponse
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
