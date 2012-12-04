#ifndef NSAPPLINKRPCV2_MESSAGETYPE_INCLUDE
#define NSAPPLINKRPCV2_MESSAGETYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
