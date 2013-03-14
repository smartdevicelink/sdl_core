#ifndef NSAPPLINKRPCV2_UPDATETURNLIST_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_UPDATETURNLIST_RESPONSE_INCLUDE

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
    class UpdateTurnList_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        UpdateTurnList_response(const UpdateTurnList_response& c);
        UpdateTurnList_response(void);

        virtual ~UpdateTurnList_response(void);

        UpdateTurnList_response& operator =(const UpdateTurnList_response&);

        bool checkIntegrity(void);

    private:
        friend class UpdateTurnList_responseMarshaller;
    };
}

#endif
