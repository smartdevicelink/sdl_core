//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_PERFORMAUDIOPASSTHRU_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_PERFORMAUDIOPASSTHRU_RESPONSE_INCLUDE

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
    class PerformAudioPassThru_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        PerformAudioPassThru_response(const PerformAudioPassThru_response& c);
        PerformAudioPassThru_response(void);

        virtual ~PerformAudioPassThru_response(void);

        PerformAudioPassThru_response& operator =(const PerformAudioPassThru_response&);

        bool checkIntegrity(void);

    private:
        friend class PerformAudioPassThru_responseMarshaller;
    };
}

#endif
