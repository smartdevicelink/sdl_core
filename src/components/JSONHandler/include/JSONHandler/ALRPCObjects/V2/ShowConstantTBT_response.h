#ifndef NSAPPLINKRPCV2_SHOWCONSTANTTBT_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_SHOWCONSTANTTBT_RESPONSE_INCLUDE

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
    class ShowConstantTBT_response : public NsAppLinkRPC::ALRPCResponse
    {
    public:
        ShowConstantTBT_response(const ShowConstantTBT_response& c);
        ShowConstantTBT_response(void);

        virtual ~ShowConstantTBT_response(void);

        ShowConstantTBT_response& operator =(const ShowConstantTBT_response&);

        bool checkIntegrity(void);

    private:
        friend class ShowConstantTBT_responseMarshaller;
    };
}

#endif
