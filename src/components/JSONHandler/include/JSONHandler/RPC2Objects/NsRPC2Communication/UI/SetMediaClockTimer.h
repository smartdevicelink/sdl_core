#ifndef NSRPC2COMMUNICATION_UI_SETMEDIACLOCKTIMER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETMEDIACLOCKTIMER_INCLUDE

#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/StartTime.h"
#include "../include/JSONHandler/ALRPCObjects/UpdateMode.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 07:25:04 2012
  source stamp	Mon Nov 19 07:04:40 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class SetMediaClockTimer : public ::RPC2Communication::RPC2Request
    {
    public:
    
      SetMediaClockTimer(const SetMediaClockTimer& c);
      SetMediaClockTimer(void);
    
      SetMediaClockTimer& operator =(const SetMediaClockTimer&);
    
      virtual ~SetMediaClockTimer(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsAppLinkRPC::StartTime* get_startTime(void);
      const NsAppLinkRPC::UpdateMode& get_updateMode(void);


// setters
      bool set_startTime(const NsAppLinkRPC::StartTime& startTime);

      void reset_startTime(void);

      bool set_updateMode(const NsAppLinkRPC::UpdateMode& updateMode);


    private:

      friend class SetMediaClockTimerMarshaller;

      NsAppLinkRPC::StartTime* startTime;
      NsAppLinkRPC::UpdateMode updateMode;

    };
  }
}

#endif
