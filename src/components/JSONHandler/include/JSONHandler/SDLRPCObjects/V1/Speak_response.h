#ifndef NSAPPLINKRPC_SPEAK_RESPONSE_INCLUDE
#define NSAPPLINKRPC_SPEAK_RESPONSE_INCLUDE

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
    class Speak_response : public SDLRPCResponse
    {
    public:
        Speak_response(const Speak_response& c);
        Speak_response(void);

        virtual ~Speak_response(void);

        Speak_response& operator =(const Speak_response&);

        bool checkIntegrity(void);

    private:
        friend class Speak_responseMarshaller;
    };
}

#endif
