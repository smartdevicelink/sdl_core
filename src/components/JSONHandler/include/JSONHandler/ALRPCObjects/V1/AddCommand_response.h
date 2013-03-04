#ifndef NSAPPLINKRPC_ADDCOMMAND_RESPONSE_INCLUDE
#define NSAPPLINKRPC_ADDCOMMAND_RESPONSE_INCLUDE

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
    class AddCommand_response : public ALRPCResponse
    {
    public:
        AddCommand_response(const AddCommand_response& c);
        AddCommand_response(void);

        virtual ~AddCommand_response(void);

        AddCommand_response& operator =(const AddCommand_response&);

        bool checkIntegrity(void);

    private:
        friend class AddCommand_responseMarshaller;
    };
}

#endif
