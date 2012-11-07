#ifndef RPC2COMMUNICATION_TTS_SPEAKRESPONSE_INCLUDE
#define RPC2COMMUNICATION_TTS_SPEAKRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	RPC2Communication::TTS
  version	1.2
  generated at	Wed Nov  7 11:25:55 2012
  source stamp	Wed Nov  7 09:34:03 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace TTS
  {

    class SpeakResponse : public ::RPC2Communication::RPC2Response
    {
    public:
    
      SpeakResponse(const SpeakResponse& c);
      SpeakResponse(void);
    
      SpeakResponse& operator =(const SpeakResponse&);
    
      virtual ~SpeakResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class SpeakResponseMarshaller;


    };
  }
}

#endif
