//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_SUBSCRIBEBUTTON_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SUBSCRIBEBUTTON_RESPONSE_INCLUDE

#include "JSONHandler/SDLRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    RC
*/

namespace NsSmartDeviceLinkRPCV2
{
    class SubscribeButton_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
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
