#ifndef NSAPPLINKRPCV2_ONAUDIOPASSTHRU_INCLUDE
#define NSAPPLINKRPCV2_ONAUDIOPASSTHRU_INCLUDE


#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  class OnAudioPassThru : public NsAppLinkRPC::ALRPCMessage
  {
  public:
  
    OnAudioPassThru(const OnAudioPassThru& c);
    OnAudioPassThru(void);
    
    virtual ~OnAudioPassThru(void);
  
    bool checkIntegrity(void);



  private:
  
    friend class OnAudioPassThruMarshaller;

  };

}

#endif
