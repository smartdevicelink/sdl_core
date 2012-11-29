#ifndef NSAPPLINKRPC_ONAUDIOPASSTHRU_INCLUDE
#define NSAPPLINKRPC_ONAUDIOPASSTHRU_INCLUDE


#include "JSONHandler/ALRPCNotification.h"


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

  class OnAudioPassThru : public ALRPCNotification
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
