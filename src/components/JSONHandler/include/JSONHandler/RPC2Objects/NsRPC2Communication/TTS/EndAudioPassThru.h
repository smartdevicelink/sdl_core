#ifndef NSRPC2COMMUNICATION_TTS_ENDAUDIOPASSTHRU_INCLUDE
#define NSRPC2COMMUNICATION_TTS_ENDAUDIOPASSTHRU_INCLUDE

#include "JSONHandler/RPC2Request.h"


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

    class EndAudioPassThru : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      EndAudioPassThru(const EndAudioPassThru& c);
      EndAudioPassThru(void);
    
      EndAudioPassThru& operator =(const EndAudioPassThru&);
    
      virtual ~EndAudioPassThru(void);
    
      bool checkIntegrity(void);
    
// getters
      int get_appId(void);


// setters
      bool set_appId(int appId);


    private:

      friend class EndAudioPassThruMarshaller;

      int appId;

    };
  }
}

#endif
