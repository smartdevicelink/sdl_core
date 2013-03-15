//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_SETMEDIACLOCKTIMER_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SETMEDIACLOCKTIMER_RESPONSE_INCLUDE

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
    class SetMediaClockTimer_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        SetMediaClockTimer_response(const SetMediaClockTimer_response& c);
        SetMediaClockTimer_response(void);

        virtual ~SetMediaClockTimer_response(void);

        SetMediaClockTimer_response& operator =(const SetMediaClockTimer_response&);

        bool checkIntegrity(void);

    private:
        friend class SetMediaClockTimer_responseMarshaller;
    };
}

#endif
