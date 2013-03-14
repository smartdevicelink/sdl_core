#ifndef NSAPPLINKRPCV2_SCROLLABLEMESSAGE_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_SCROLLABLEMESSAGE_RESPONSE_INCLUDE

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
    class ScrollableMessage_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        ScrollableMessage_response(const ScrollableMessage_response& c);
        ScrollableMessage_response(void);

        virtual ~ScrollableMessage_response(void);

        ScrollableMessage_response& operator =(const ScrollableMessage_response&);

        bool checkIntegrity(void);

    private:
        friend class ScrollableMessage_responseMarshaller;
    };
}

#endif
