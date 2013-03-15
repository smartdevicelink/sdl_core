//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_ALERT_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPC_ALERT_RESPONSE_INCLUDE

#include "JSONHandler/SDLRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   1.2
  date      2011-05-17
  generated at  Thu Jan 24 06:36:21 2013
  source stamp  Thu Jan 24 06:35:34 2013
  author    RC
*/

namespace NsSmartDeviceLinkRPC
{
    class Alert_response : public SDLRPCResponse
    {
    public:
        Alert_response(const Alert_response& c);
        Alert_response(void);

        virtual ~Alert_response(void);

        Alert_response& operator =(const Alert_response&);

        bool checkIntegrity(void);

    private:
        friend class Alert_responseMarshaller;
    };
}

#endif
