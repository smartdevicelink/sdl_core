//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_SETAPPICON_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SETAPPICON_RESPONSE_INCLUDE

#include "JSONHandler/SDLRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    RC
*/

namespace NsSmartDeviceLinkRPCV2
{
    /**
         Response is sent, when the file data was copied (success case). Or when an error occured.
         Not supported on First generation SYNC vehicles.
         Response is sent, when the file data was copied (success case). Or when an error occured.
         Not supported on First generation SYNC vehicles.
    */
    class SetAppIcon_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        SetAppIcon_response(const SetAppIcon_response& c);
        SetAppIcon_response(void);

        virtual ~SetAppIcon_response(void);

        SetAppIcon_response& operator =(const SetAppIcon_response&);

        bool checkIntegrity(void);

    private:
        friend class SetAppIcon_responseMarshaller;
    };
}

#endif
