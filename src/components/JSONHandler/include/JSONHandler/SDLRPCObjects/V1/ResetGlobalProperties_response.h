//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_RESETGLOBALPROPERTIES_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPC_RESETGLOBALPROPERTIES_RESPONSE_INCLUDE

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
    class ResetGlobalProperties_response : public SDLRPCResponse
    {
    public:
        ResetGlobalProperties_response(const ResetGlobalProperties_response& c);
        ResetGlobalProperties_response(void);

        virtual ~ResetGlobalProperties_response(void);

        ResetGlobalProperties_response& operator =(const ResetGlobalProperties_response&);

        bool checkIntegrity(void);

    private:
        friend class ResetGlobalProperties_responseMarshaller;
    };
}

#endif
