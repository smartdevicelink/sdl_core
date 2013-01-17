#ifndef NSRPC2COMMUNICATION_UI_ENDAUDIOPASSTHRURESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_ENDAUDIOPASSTHRURESPONSE_INCLUDE

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

    class EndAudioPassThruResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      EndAudioPassThruResponse(const EndAudioPassThruResponse& c);
      EndAudioPassThruResponse(void);
    
      EndAudioPassThruResponse& operator =(const EndAudioPassThruResponse&);
    
      virtual ~EndAudioPassThruResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class EndAudioPassThruResponseMarshaller;


    };
  }
}

#endif
