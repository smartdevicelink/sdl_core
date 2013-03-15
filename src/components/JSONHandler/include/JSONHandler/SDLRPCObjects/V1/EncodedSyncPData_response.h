//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_ENCODEDSYNCPDATA_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPC_ENCODEDSYNCPDATA_RESPONSE_INCLUDE

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
    class EncodedSyncPData_response : public SDLRPCResponse
    {
    public:
        EncodedSyncPData_response(const EncodedSyncPData_response& c);
        EncodedSyncPData_response(void);

        virtual ~EncodedSyncPData_response(void);

        EncodedSyncPData_response& operator =(const EncodedSyncPData_response&);

        bool checkIntegrity(void);

    private:
        friend class EncodedSyncPData_responseMarshaller;
    };
}

#endif
