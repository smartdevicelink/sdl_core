//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_ALERT_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_ALERT_RESPONSE_INCLUDE

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
    class Alert_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        Alert_response(const Alert_response& c);
        Alert_response(void);

        virtual ~Alert_response(void);

        Alert_response& operator =(const Alert_response&);

        bool checkIntegrity(void);

        unsigned int get_tryAgainTime(void) const;
        bool set_tryAgainTime(unsigned int tryAgainTime_);

    private:
        friend class Alert_responseMarshaller;

        /**
             Amount of time (in seconds) that an app must wait before resending an alert.
             If provided, another system event or overlay currently has a higher priority than this alert.
             An app must not send an alert without waiting at least the amount of time dictated.
        */
        unsigned int tryAgainTime;    //!<  (0,2000000000)
    };
}

#endif
