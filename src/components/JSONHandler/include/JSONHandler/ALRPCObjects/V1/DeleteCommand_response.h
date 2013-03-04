#ifndef NSAPPLINKRPC_DELETECOMMAND_RESPONSE_INCLUDE
#define NSAPPLINKRPC_DELETECOMMAND_RESPONSE_INCLUDE

#include "JSONHandler/ALRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   1.2
  date      2011-05-17
  generated at  Thu Jan 24 06:36:21 2013
  source stamp  Thu Jan 24 06:35:34 2013
  author    robok0der
*/

namespace NsAppLinkRPC
{
    class DeleteCommand_response : public ALRPCResponse
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
