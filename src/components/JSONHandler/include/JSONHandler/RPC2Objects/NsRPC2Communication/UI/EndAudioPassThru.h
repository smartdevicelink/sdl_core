//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_ENDAUDIOPASSTHRU_INCLUDE
#define NSRPC2COMMUNICATION_UI_ENDAUDIOPASSTHRU_INCLUDE

#include "JSONHandler/RPC2Request.h"


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
