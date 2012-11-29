#ifndef NSAPPLINKRPC_ENDAUDIOPASSTHRU_REQUEST_INCLUDE
#define NSAPPLINKRPC_ENDAUDIOPASSTHRU_REQUEST_INCLUDE


#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  When this request is invoked, the audio capture stops.

  class EndAudioPassThru_request : public ALRPCRequest
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
