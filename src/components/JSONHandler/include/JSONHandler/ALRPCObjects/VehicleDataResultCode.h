#ifndef VEHICLEDATARESULTCODE_INCLUDE
#define VEHICLEDATARESULTCODE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


///  Enumeration that describes possible result codes of a vehicle data entry request.

class VehicleDataResultCode
{
public:
  enum VehicleDataResultCodeInternal
  {
    INVALID_ENUM=-1,
    VDRC_SUCCESS=0,
    VDRC_DISALLOWED=1,
    VDRC_USER_DISALLOWED=2,
    VDRC_INVALID_ID=3,
    VDRC_DATA_NOT_AVAILABLE=4,
    VDRC_DATA_ALREADY_SUBSCRIBED=5
  };

  VehicleDataResultCode() : mInternal(INVALID_ENUM)				{}
  VehicleDataResultCode(VehicleDataResultCodeInternal e) : mInternal(e)		{}

  VehicleDataResultCodeInternal get(void) const	{ return mInternal; }
  void set(VehicleDataResultCodeInternal e)		{ mInternal=e; }

private:
  VehicleDataResultCodeInternal mInternal;
  friend class VehicleDataResultCodeMarshaller;
};

#endif
