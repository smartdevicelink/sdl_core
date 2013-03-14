#ifndef NSAPPLINKRPC_SETMEDIACLOCKTIMER_RESPONSE_INCLUDE
#define NSAPPLINKRPC_SETMEDIACLOCKTIMER_RESPONSE_INCLUDE

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
    class SetMediaClockTimer_response : public SDLRPCResponse
    {
    public:
        SetMediaClockTimer_response(const SetMediaClockTimer_response& c);
        SetMediaClockTimer_response(void);

        virtual ~SetMediaClockTimer_response(void);

        SetMediaClockTimer_response& operator =(const SetMediaClockTimer_response&);

        bool checkIntegrity(void);

    private:
        friend class SetMediaClockTimer_responseMarshaller;
    };
}

#endif
