#ifndef NSAPPLINKRPCV2_ONCOMMAND_INCLUDE
#define NSAPPLINKRPCV2_ONCOMMAND_INCLUDE


#include "TriggerSource.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  class OnCommand : public NsAppLinkRPC::ALRPCMessage
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
