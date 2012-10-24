#ifndef MESSAGETYPE_INCLUDE
#define MESSAGETYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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

#endif
