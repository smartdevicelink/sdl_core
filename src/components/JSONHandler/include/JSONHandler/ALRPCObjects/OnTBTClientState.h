#ifndef ONTBTCLIENTSTATE_INCLUDE
#define ONTBTCLIENTSTATE_INCLUDE


#include "TBTState.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


///  Provides applications with notifications specific to the current TBT client status on the module

class OnTBTClientState : public ALRPCNotification
{
public:

  OnTBTClientState(const OnTBTClientState& c);
  OnTBTClientState(void);
  
  virtual ~OnTBTClientState(void);

  bool checkIntegrity(void);

  const TBTState& get_state(void) const;

  bool set_state(const TBTState& state_);

private:

  friend class OnTBTClientStateMarshaller;

  TBTState state;
};

#endif
