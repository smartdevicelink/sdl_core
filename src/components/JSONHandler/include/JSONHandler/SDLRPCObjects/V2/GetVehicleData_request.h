#ifndef NSAPPLINKRPCV2_GETVEHICLEDATA_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_GETVEHICLEDATA_REQUEST_INCLUDE


#include "VehicleDataType.h"
#include "JSONHandler/SDLRPCMessage.h"


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

///  Non periodic vehicle data read request.

  class GetVehicleData_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    GetVehicleData_request(const GetVehicleData_request& c);
    GetVehicleData_request(void);
    
    virtual ~GetVehicleData_request(void);
  
    bool checkIntegrity(void);

    const VehicleDataType& get_dataType(void) const;

    bool set_dataType(const VehicleDataType& dataType_);

  private:
  
    friend class GetVehicleData_requestMarshaller;


///  Complex signals like "GPS" should not be transferred as separate notifications, but all together in one notification.
      VehicleDataType dataType;
  };

}

#endif
