#ifndef ONTBTCLIENTSTATE_INCLUDE
#define ONTBTCLIENTSTATE_INCLUDE


#include "TBTState.h"
#include "../../../JSONHandler/ALRPCNotification.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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
