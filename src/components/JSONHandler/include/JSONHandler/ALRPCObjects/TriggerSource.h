#ifndef TRIGGERSOURCE_INCLUDE
#define TRIGGERSOURCE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


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

#endif
