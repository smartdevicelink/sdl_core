#ifndef ONCOMMAND_INCLUDE
#define ONCOMMAND_INCLUDE


#include "TriggerSource.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


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

  unsigned int cmdID;	//!<  (0,2000000000)
  TriggerSource triggerSource;
};

#endif
