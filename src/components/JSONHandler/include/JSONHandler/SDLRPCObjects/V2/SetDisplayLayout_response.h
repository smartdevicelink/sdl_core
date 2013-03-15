//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_SETDISPLAYLAYOUT_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SETDISPLAYLAYOUT_RESPONSE_INCLUDE

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
    class SetDisplayLayout_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        SetDisplayLayout_response(const SetDisplayLayout_response& c);
        SetDisplayLayout_response(void);

        virtual ~SetDisplayLayout_response(void);

        SetDisplayLayout_response& operator =(const SetDisplayLayout_response&);

        bool checkIntegrity(void);

    private:
        friend class SetDisplayLayout_responseMarshaller;
    };
}

#endif
