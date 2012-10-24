#ifndef TIREPRESSURETELLTALE_INCLUDE
#define TIREPRESSURETELLTALE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


///  Reflects the status of the cluster instrument warning light.

class TirePressureTellTale
{
public:
  enum TirePressureTellTaleInternal
  {
    INVALID_ENUM=-1,
    OFF=0,
    ON=1,
    FLASH=2
  };

  TirePressureTellTale() : mInternal(INVALID_ENUM)				{}
  TirePressureTellTale(TirePressureTellTaleInternal e) : mInternal(e)		{}

  TirePressureTellTaleInternal get(void) const	{ return mInternal; }
  void set(TirePressureTellTaleInternal e)		{ mInternal=e; }

private:
  TirePressureTellTaleInternal mInternal;
  friend class TirePressureTellTaleMarshaller;
};

#endif
