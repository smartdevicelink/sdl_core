//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_ONCOMMAND_INCLUDE
#define NSSMARTDEVICELINKKRPC_ONCOMMAND_INCLUDE


#include "TriggerSource.h"
#include "JSONHandler/SDLRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPC
{

  class OnCommand : public SDLRPCNotification
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
