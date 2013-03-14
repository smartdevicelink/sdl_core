#ifndef NSAPPLINKRPC_SHOW_RESPONSE_INCLUDE
#define NSAPPLINKRPC_SHOW_RESPONSE_INCLUDE

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
    class Show_response : public SDLRPCResponse
    {
    public:
        Show_response(const Show_response& c);
        Show_response(void);

        virtual ~Show_response(void);

        Show_response& operator =(const Show_response&);

        bool checkIntegrity(void);

    private:
        friend class Show_responseMarshaller;
    };
}

#endif
