#ifndef ONAPPINTERFACEUNREGISTERED_INCLUDE
#define ONAPPINTERFACEUNREGISTERED_INCLUDE


#include "AppInterfaceUnregisteredReason.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
