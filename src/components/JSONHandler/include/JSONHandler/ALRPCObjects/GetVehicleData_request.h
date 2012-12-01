#ifndef NSAPPLINKRPC_GETVEHICLEDATA_REQUEST_INCLUDE
#define NSAPPLINKRPC_GETVEHICLEDATA_REQUEST_INCLUDE


#include "VehicleDataType.h"
#include "JSONHandler/ALRPCRequest.h"


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

///  Non periodic vehicle data read request.

  class GetVehicleData_request : public ALRPCRequest
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
