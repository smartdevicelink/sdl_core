//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_DELETECOMMAND_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_DELETECOMMAND_RESPONSE_INCLUDE

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
    class DeleteCommand_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        DeleteCommand_response(const DeleteCommand_response& c);
        DeleteCommand_response(void);

        virtual ~DeleteCommand_response(void);

        DeleteCommand_response& operator =(const DeleteCommand_response&);

        bool checkIntegrity(void);

    private:
        friend class DeleteCommand_responseMarshaller;
    };
}

#endif
