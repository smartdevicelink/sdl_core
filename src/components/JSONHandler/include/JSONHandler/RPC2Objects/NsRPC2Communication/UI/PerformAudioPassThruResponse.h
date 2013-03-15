//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_PERFORMAUDIOPASSTHRURESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_PERFORMAUDIOPASSTHRURESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
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
