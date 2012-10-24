#ifndef SINGLETIREPRESSURESTATUS_INCLUDE
#define SINGLETIREPRESSURESTATUS_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


///  The pressure status of a tire.

class SingleTirePressureStatus
{
public:
  enum SingleTirePressureStatusInternal
  {
    INVALID_ENUM=-1,
    UNKNOWN=0,
    NORMAL=1,
    LOW=2,
    FAULT=3,
    NOT_SUPPORTED=4
  };

  SingleTirePressureStatus() : mInternal(INVALID_ENUM)				{}
  SingleTirePressureStatus(SingleTirePressureStatusInternal e) : mInternal(e)		{}

  SingleTirePressureStatusInternal get(void) const	{ return mInternal; }
  void set(SingleTirePressureStatusInternal e)		{ mInternal=e; }

private:
  SingleTirePressureStatusInternal mInternal;
  friend class SingleTirePressureStatusMarshaller;
};

#endif
