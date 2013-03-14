#ifndef NSAPPLINKRPCV2_ADDCOMMAND_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_ADDCOMMAND_RESPONSE_INCLUDE

#include "JSONHandler/SDLRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    robok0der
*/

namespace NsSmartDeviceLinkRPCV2
{
    class AddCommand_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
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
