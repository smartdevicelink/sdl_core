//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_CHANGEREGISTRATION_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_CHANGEREGISTRATION_RESPONSE_INCLUDE

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
    class ChangeRegistration_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        ChangeRegistration_response(const ChangeRegistration_response& c);
        ChangeRegistration_response(void);

        virtual ~ChangeRegistration_response(void);

        ChangeRegistration_response& operator =(const ChangeRegistration_response&);

        bool checkIntegrity(void);

    private:
        friend class ChangeRegistration_responseMarshaller;
    };
}

#endif
