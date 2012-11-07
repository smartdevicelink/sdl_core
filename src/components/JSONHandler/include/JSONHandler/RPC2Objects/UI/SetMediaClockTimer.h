#ifndef RPC2COMMUNICATION_UI_SETMEDIACLOCKTIMER_INCLUDE
#define RPC2COMMUNICATION_UI_SETMEDIACLOCKTIMER_INCLUDE

#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/StartTime.h"
#include "../include/JSONHandler/ALRPCObjects/UpdateMode.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

namespace RPC2Communication
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
  const     ::AppLinkRPC::StartTime* get_startTime(void);
      const ::AppLinkRPC::UpdateMode& get_updateMode(void);


// setters
      bool set_startTime(const ::AppLinkRPC::StartTime& startTime);

      void reset_startTime(void);

      bool set_updateMode(const ::AppLinkRPC::UpdateMode& updateMode);


    private:

      friend class SetMediaClockTimerMarshaller;

      ::AppLinkRPC::StartTime* startTime;
      ::AppLinkRPC::UpdateMode updateMode;

    };
  }
}

#endif
