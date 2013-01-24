#ifndef NSRPC2COMMUNICATION_UI_SETMEDIACLOCKTIMER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETMEDIACLOCKTIMER_INCLUDE

#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/V2/StartTime.h"
#include "../include/JSONHandler/ALRPCObjects/V2/UpdateMode.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class SetMediaClockTimer : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      SetMediaClockTimer(const SetMediaClockTimer& c);
      SetMediaClockTimer(void);
    
      SetMediaClockTimer& operator =(const SetMediaClockTimer&);
    
      virtual ~SetMediaClockTimer(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsAppLinkRPCV2::StartTime* get_startTime(void);
      const NsAppLinkRPCV2::UpdateMode& get_updateMode(void);

      int get_appId(void);


// setters
      bool set_startTime(const NsAppLinkRPCV2::StartTime& startTime);

      void reset_startTime(void);

      bool set_updateMode(const NsAppLinkRPCV2::UpdateMode& updateMode);

      bool set_appId(int appId);


    private:

      friend class SetMediaClockTimerMarshaller;

      NsAppLinkRPCV2::StartTime* startTime;
      NsAppLinkRPCV2::UpdateMode updateMode;
      int appId;

    };
  }
}

#endif
