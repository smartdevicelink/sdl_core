#ifndef NSAPPLINKRPCV2_ONAPPINTERFACEUNREGISTERED_INCLUDE
#define NSAPPLINKRPCV2_ONAPPINTERFACEUNREGISTERED_INCLUDE


#include "AppInterfaceUnregisteredReason.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  class OnAppInterfaceUnregistered : public NsAppLinkRPC::ALRPCMessage
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
