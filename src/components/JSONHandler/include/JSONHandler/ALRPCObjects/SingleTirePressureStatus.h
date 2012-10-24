#ifndef SINGLETIREPRESSURESTATUS_INCLUDE
#define SINGLETIREPRESSURESTATUS_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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
