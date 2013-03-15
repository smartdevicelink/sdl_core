//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_CREATEINTERACTIONCHOICESET_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPC_CREATEINTERACTIONCHOICESET_RESPONSE_INCLUDE

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
    class CreateInteractionChoiceSet_response : public SDLRPCResponse
    {
    public:
        CreateInteractionChoiceSet_response(const CreateInteractionChoiceSet_response& c);
        CreateInteractionChoiceSet_response(void);

        virtual ~CreateInteractionChoiceSet_response(void);

        CreateInteractionChoiceSet_response& operator =(const CreateInteractionChoiceSet_response&);

        bool checkIntegrity(void);

    private:
        friend class CreateInteractionChoiceSet_responseMarshaller;
    };
}

#endif
