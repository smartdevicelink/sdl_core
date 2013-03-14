#ifndef NSAPPLINKRPCV2_VEHICLEDATARESULTCODE_INCLUDE
#define NSAPPLINKRPCV2_VEHICLEDATARESULTCODE_INCLUDE


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
      VDRC_DATA_ALREADY_SUBSCRIBED=5,
      VDRC_DATA_NOT_SUBSCRIBED=6,
      VDRC_IGNORED=7
    };
  
    VehicleDataResultCode() : mInternal(INVALID_ENUM)				{}
    VehicleDataResultCode(VehicleDataResultCodeInternal e) : mInternal(e)		{}
  
    VehicleDataResultCodeInternal get(void) const	{ return mInternal; }
    void set(VehicleDataResultCodeInternal e)		{ mInternal=e; }
  
  private:
    VehicleDataResultCodeInternal mInternal;
    friend class VehicleDataResultCodeMarshaller;
  };
  
}

#endif
