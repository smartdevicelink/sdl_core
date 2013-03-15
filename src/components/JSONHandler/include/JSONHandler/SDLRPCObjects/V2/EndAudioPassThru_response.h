//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_ENDAUDIOPASSTHRU_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_ENDAUDIOPASSTHRU_RESPONSE_INCLUDE

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
    class EndAudioPassThru_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        EndAudioPassThru_response(const EndAudioPassThru_response& c);
        EndAudioPassThru_response(void);

        virtual ~EndAudioPassThru_response(void);

        EndAudioPassThru_response& operator =(const EndAudioPassThru_response&);

        bool checkIntegrity(void);

    private:
        friend class EndAudioPassThru_responseMarshaller;
    };
}

#endif
