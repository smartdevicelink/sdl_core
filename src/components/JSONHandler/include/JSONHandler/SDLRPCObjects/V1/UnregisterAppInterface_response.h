//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_UNREGISTERAPPINTERFACE_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPC_UNREGISTERAPPINTERFACE_RESPONSE_INCLUDE

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
    class UnregisterAppInterface_response : public SDLRPCResponse
    {
    public:
        UnregisterAppInterface_response(const UnregisterAppInterface_response& c);
        UnregisterAppInterface_response(void);

        virtual ~UnregisterAppInterface_response(void);

        UnregisterAppInterface_response& operator =(const UnregisterAppInterface_response&);

        bool checkIntegrity(void);

    private:
        friend class UnregisterAppInterface_responseMarshaller;
    };
}

#endif
