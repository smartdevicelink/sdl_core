//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_TRIGGERSOURCE_INCLUDE
#define NSSMARTDEVICELINKKRPC_TRIGGERSOURCE_INCLUDE


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

///  Indicates the source from where the command was triggered.

  class TriggerSource
  {
  public:
    enum TriggerSourceInternal
    {
      INVALID_ENUM=-1,
      TS_MENU=0,
      TS_VR=1
    };
  
    TriggerSource() : mInternal(INVALID_ENUM)				{}
    TriggerSource(TriggerSourceInternal e) : mInternal(e)		{}
  
    TriggerSourceInternal get(void) const	{ return mInternal; }
    void set(TriggerSourceInternal e)		{ mInternal=e; }
  
  private:
    TriggerSourceInternal mInternal;
    friend class TriggerSourceMarshaller;
  };
  
}

#endif
