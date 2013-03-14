#ifndef NSAPPLINKRPCV2_MESSAGETYPE_INCLUDE
#define NSAPPLINKRPCV2_MESSAGETYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPCV2
{

/**
     Enumeration linking message types with function types in WiPro protocol.
     Assumes enumeration starts at value 0.
*/

  class messageType
  {
  public:
    enum messageTypeInternal
    {
      INVALID_ENUM=-1,
      request=0,
      response=1,
      notification=2
    };
  
    messageType() : mInternal(INVALID_ENUM)				{}
    messageType(messageTypeInternal e) : mInternal(e)		{}
  
    messageTypeInternal get(void) const	{ return mInternal; }
    void set(messageTypeInternal e)		{ mInternal=e; }
  
  private:
    messageTypeInternal mInternal;
    friend class messageTypeMarshaller;
  };
  
}

#endif
