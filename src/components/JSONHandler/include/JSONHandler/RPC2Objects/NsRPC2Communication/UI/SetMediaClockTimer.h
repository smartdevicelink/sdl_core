#ifndef NSRPC2COMMUNICATION_UI_SETMEDIACLOCKTIMER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETMEDIACLOCKTIMER_INCLUDE

#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/V1/StartTime.h"
#include "../include/JSONHandler/ALRPCObjects/V1/UpdateMode.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
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
      const NsAppLinkRPC::StartTime* get_startTime(void);
      const NsAppLinkRPC::UpdateMode& get_updateMode(void);

      int get_appId(void);


// setters
      bool set_startTime(const NsAppLinkRPC::StartTime& startTime);

      void reset_startTime(void);

      bool set_updateMode(const NsAppLinkRPC::UpdateMode& updateMode);

      bool set_appId(int appId);


    private:

      friend class SetMediaClockTimerMarshaller;

      NsAppLinkRPC::StartTime* startTime;
      NsAppLinkRPC::UpdateMode updateMode;
      int appId;

    };
  }
}

#endif
