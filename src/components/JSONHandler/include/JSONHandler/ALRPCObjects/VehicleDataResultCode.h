#ifndef NSAPPLINKRPC_VEHICLEDATARESULTCODE_INCLUDE
#define NSAPPLINKRPC_VEHICLEDATARESULTCODE_INCLUDE


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
