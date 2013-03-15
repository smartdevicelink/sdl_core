//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_ENDAUDIOPASSTHRU_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_ENDAUDIOPASSTHRU_REQUEST_INCLUDE


#include "JSONHandler/SDLRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{

///  When this request is invoked, the audio capture stops.

  class EndAudioPassThru_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    EndAudioPassThru_request(const EndAudioPassThru_request& c);
    EndAudioPassThru_request(void);
    
    virtual ~EndAudioPassThru_request(void);
  
    bool checkIntegrity(void);



  private:
  
    friend class EndAudioPassThru_requestMarshaller;

  };

}

#endif
