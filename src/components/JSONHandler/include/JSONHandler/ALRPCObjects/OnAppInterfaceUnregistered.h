#ifndef ONAPPINTERFACEUNREGISTERED_INCLUDE
#define ONAPPINTERFACEUNREGISTERED_INCLUDE


#include "AppInterfaceUnregisteredReason.h"
#include "../../../JSONHandler/ALRPCNotification.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


class OnAppInterfaceUnregistered : public ALRPCNotification
{
public:

  OnAppInterfaceUnregistered(const OnAppInterfaceUnregistered& c);
  OnAppInterfaceUnregistered(void);
  
  virtual ~OnAppInterfaceUnregistered(void);

  bool checkIntegrity(void);

  const AppInterfaceUnregisteredReason& get_reason(void) const;

  bool set_reason(const AppInterfaceUnregisteredReason& reason_);

private:

  friend class OnAppInterfaceUnregisteredMarshaller;

  AppInterfaceUnregisteredReason reason;
};

#endif
