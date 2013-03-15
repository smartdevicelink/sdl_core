//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_ADDSUBMENU_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPC_ADDSUBMENU_RESPONSE_INCLUDE

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
    class AddSubMenu_response : public SDLRPCResponse
    {
    public:
        AddSubMenu_response(const AddSubMenu_response& c);
        AddSubMenu_response(void);

        virtual ~AddSubMenu_response(void);

        AddSubMenu_response& operator =(const AddSubMenu_response&);

        bool checkIntegrity(void);

    private:
        friend class AddSubMenu_responseMarshaller;
    };
}

#endif
