#ifndef APPLINKRPC_ONAPPINTERFACEUNREGISTERED_INCLUDE
#define APPLINKRPC_ONAPPINTERFACEUNREGISTERED_INCLUDE


#include "AppInterfaceUnregisteredReason.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

namespace AppLinkRPC
{

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


///  See AppInterfaceUnregisteredReason
      AppInterfaceUnregisteredReason reason;
  };

}

#endif
