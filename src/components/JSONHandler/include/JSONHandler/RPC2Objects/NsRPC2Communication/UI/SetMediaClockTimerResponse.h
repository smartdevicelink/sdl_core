#ifndef NSRPC2COMMUNICATION_UI_SETMEDIACLOCKTIMERRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETMEDIACLOCKTIMERRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class SetMediaClockTimerResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      SetMediaClockTimerResponse(const SetMediaClockTimerResponse& c);
      SetMediaClockTimerResponse(void);
    
      SetMediaClockTimerResponse& operator =(const SetMediaClockTimerResponse&);
    
      virtual ~SetMediaClockTimerResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class SetMediaClockTimerResponseMarshaller;


    };
  }
}

#endif
