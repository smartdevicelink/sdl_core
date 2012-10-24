#ifndef SETMEDIACLOCKTIMER_REQUEST_INCLUDE
#define SETMEDIACLOCKTIMER_REQUEST_INCLUDE


#include "StartTime.h"
#include "UpdateMode.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


///  Sets the initial media clock value and automatic update method.

class SetMediaClockTimer_request : public ALRPCRequest
{
public:

  SetMediaClockTimer_request(const SetMediaClockTimer_request& c);
  SetMediaClockTimer_request(void);
  
  virtual ~SetMediaClockTimer_request(void);

  SetMediaClockTimer_request& operator =(const SetMediaClockTimer_request&);

  bool checkIntegrity(void);

  const StartTime* get_startTime(void) const;
  const UpdateMode& get_updateMode(void) const;

  void reset_startTime(void);
  bool set_startTime(const StartTime& startTime_);
  bool set_updateMode(const UpdateMode& updateMode_);

private:

  friend class SetMediaClockTimer_requestMarshaller;

  StartTime* startTime;
  UpdateMode updateMode;
};

#endif
