#ifndef NSAPPLINKRPC_DELETESUBMENU_RESPONSE_INCLUDE
#define NSAPPLINKRPC_DELETESUBMENU_RESPONSE_INCLUDE

#include "JSONHandler/SDLRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   1.2
  date      2011-05-17
  generated at  Thu Jan 24 06:36:21 2013
  source stamp  Thu Jan 24 06:35:34 2013
  author    robok0der
*/

namespace NsSmartDeviceLinkRPC
{
    class DeleteSubMenu_response : public SDLRPCResponse
    {
    public:
        DeleteSubMenu_response(const DeleteSubMenu_response& c);
        DeleteSubMenu_response(void);

        virtual ~DeleteSubMenu_response(void);

        DeleteSubMenu_response& operator =(const DeleteSubMenu_response&);

        bool checkIntegrity(void);

    private:
        friend class DeleteSubMenu_responseMarshaller;
    };
}

#endif
