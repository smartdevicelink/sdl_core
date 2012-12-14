#ifndef NSRPC2COMMUNICATION_TTS_ONAUDIOPASSTHRU_INCLUDE
#define NSRPC2COMMUNICATION_TTS_ONAUDIOPASSTHRU_INCLUDE

#include "JSONHandler/RPC2Notification.h"


/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace TTS
  {

    class OnAudioPassThru : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnAudioPassThru(const OnAudioPassThru& c);
      OnAudioPassThru(void);
    
      OnAudioPassThru& operator =(const OnAudioPassThru&);
    
      virtual ~OnAudioPassThru(void);
    
      bool checkIntegrity(void);
    
// getters
      int get_appId(void);


// setters
      bool set_appId(int appId);


    private:

      friend class OnAudioPassThruMarshaller;

      int appId;

    };
  }
}

#endif
