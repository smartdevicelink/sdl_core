#ifndef NSAPPLINKRPCV2_CREATEINTERACTIONCHOICESET_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_CREATEINTERACTIONCHOICESET_RESPONSE_INCLUDE

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
    class CreateInteractionChoiceSet_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
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
