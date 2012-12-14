#ifndef NSAPPLINKRPC_ONCOMMAND_INCLUDE
#define NSAPPLINKRPC_ONCOMMAND_INCLUDE


#include "TriggerSource.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  class OnCommand : public ALRPCNotification
  {
  public:
  
    OnCommand(const OnCommand& c);
    OnCommand(void);
    
    virtual ~OnCommand(void);
  
    bool checkIntegrity(void);

    unsigned int get_cmdID(void) const;
    const TriggerSource& get_triggerSource(void) const;

    bool set_cmdID(unsigned int cmdID_);
    bool set_triggerSource(const TriggerSource& triggerSource_);

  private:
  
    friend class OnCommandMarshaller;


///  Command ID, which is related to a specific menu entry.
      unsigned int cmdID;	//!<  (0,2000000000)

///  See TriggerSource
      TriggerSource triggerSource;
  };

}

#endif
