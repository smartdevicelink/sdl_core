#ifndef NSAPPLINKRPCV2_ENDAUDIOPASSTHRU_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_ENDAUDIOPASSTHRU_REQUEST_INCLUDE


#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  When this request is invoked, the audio capture stops.

  class EndAudioPassThru_request : public NsAppLinkRPC::ALRPCMessage
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
