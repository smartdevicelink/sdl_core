//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_SETGLOBALPROPERTIES_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SETGLOBALPROPERTIES_RESPONSE_INCLUDE

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
    class SetGlobalProperties_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        SetGlobalProperties_response(const SetGlobalProperties_response& c);
        SetGlobalProperties_response(void);

        virtual ~SetGlobalProperties_response(void);

        SetGlobalProperties_response& operator =(const SetGlobalProperties_response&);

        bool checkIntegrity(void);

    private:
        friend class SetGlobalProperties_responseMarshaller;
    };
}

#endif
