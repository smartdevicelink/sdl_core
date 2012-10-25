#ifndef ONAPPINTERFACEUNREGISTERED_INCLUDE
#define ONAPPINTERFACEUNREGISTERED_INCLUDE


#include "AppInterfaceUnregisteredReason.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
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
