#ifndef NSAPPLINKRPCV2_GETVEHICLEDATA_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_GETVEHICLEDATA_REQUEST_INCLUDE


#include "VehicleDataType.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Non periodic vehicle data read request.

  class GetVehicleData_request : public NsAppLinkRPC::ALRPCMessage
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
