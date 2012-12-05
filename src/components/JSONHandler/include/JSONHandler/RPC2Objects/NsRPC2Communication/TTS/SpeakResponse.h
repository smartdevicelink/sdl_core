#ifndef NSRPC2COMMUNICATION_TTS_SPEAKRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_TTS_SPEAKRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace TTS
  {

    class SpeakResponse : public ::NsRPC2Communication::RPC2Response
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
