#ifndef NSAPPLINKRPC_ONAPPINTERFACEUNREGISTERED_V2_INCLUDE
#define NSAPPLINKRPC_ONAPPINTERFACEUNREGISTERED_V2_INCLUDE


#include "AppInterfaceUnregisteredReason_v2.h"
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

  class OnAppInterfaceUnregistered_v2 : public ALRPCNotification
  {
  public:
  
    OnAppInterfaceUnregistered_v2(const OnAppInterfaceUnregistered_v2& c);
    OnAppInterfaceUnregistered_v2(void);
    
    virtual ~OnAppInterfaceUnregistered_v2(void);
  
    bool checkIntegrity(void);

    const AppInterfaceUnregisteredReason_v2& get_reason(void) const;

    bool set_reason(const AppInterfaceUnregisteredReason_v2& reason_);

  private:
  
    friend class OnAppInterfaceUnregistered_v2Marshaller;


///  See AppInterfaceUnregisteredReason
      AppInterfaceUnregisteredReason_v2 reason;
  };

}

#endif
