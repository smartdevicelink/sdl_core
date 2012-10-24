#ifndef TRIGGERSOURCE_INCLUDE
#define TRIGGERSOURCE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
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
