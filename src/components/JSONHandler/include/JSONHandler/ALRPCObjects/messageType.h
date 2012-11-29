#ifndef NSAPPLINKRPC_MESSAGETYPE_INCLUDE
#define NSAPPLINKRPC_MESSAGETYPE_INCLUDE


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
