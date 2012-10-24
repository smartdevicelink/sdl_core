#ifndef GETVEHICLEDATA_REQUEST_INCLUDE
#define GETVEHICLEDATA_REQUEST_INCLUDE


#include "VehicleDataType.h"
#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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

  VehicleDataType dataType;
};

#endif
