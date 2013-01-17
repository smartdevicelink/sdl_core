#ifndef NSRPC2COMMUNICATION_TTS_PERFORMAUDIOPASSTHRURESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_TTS_PERFORMAUDIOPASSTHRURESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class PerformAudioPassThruResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      PerformAudioPassThruResponse(const PerformAudioPassThruResponse& c);
      PerformAudioPassThruResponse(void);
    
      PerformAudioPassThruResponse& operator =(const PerformAudioPassThruResponse&);
    
      virtual ~PerformAudioPassThruResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class PerformAudioPassThruResponseMarshaller;


    };
  }
}

#endif
