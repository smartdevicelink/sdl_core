#ifndef NSAPPLINKRPC_ONAPPINTERFACEUNREGISTERED_INCLUDE
#define NSAPPLINKRPC_ONAPPINTERFACEUNREGISTERED_INCLUDE


#include "AppInterfaceUnregisteredReason.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
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
