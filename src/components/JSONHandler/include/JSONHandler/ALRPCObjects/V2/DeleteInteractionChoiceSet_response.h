#ifndef NSAPPLINKRPCV2_DELETEINTERACTIONCHOICESET_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_DELETEINTERACTIONCHOICESET_RESPONSE_INCLUDE

#include "JSONHandler/ALRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    robok0der
*/

namespace NsAppLinkRPCV2
{
    class DeleteInteractionChoiceSet_response : public NsAppLinkRPC::ALRPCResponse
    {
    public:
        DeleteInteractionChoiceSet_response(const DeleteInteractionChoiceSet_response& c);
        DeleteInteractionChoiceSet_response(void);

        virtual ~DeleteInteractionChoiceSet_response(void);

        DeleteInteractionChoiceSet_response& operator =(const DeleteInteractionChoiceSet_response&);

        bool checkIntegrity(void);

    private:
        friend class DeleteInteractionChoiceSet_responseMarshaller;
    };
}

#endif
