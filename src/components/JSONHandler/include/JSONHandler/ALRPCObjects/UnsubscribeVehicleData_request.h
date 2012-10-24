#ifndef UNSUBSCRIBEVEHICLEDATA_REQUEST_INCLUDE
#define UNSUBSCRIBEVEHICLEDATA_REQUEST_INCLUDE

#include <vector>

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


///  This function is used to unsubscribe the notifications from the subscribeVehicleData function.

class UnsubscribeVehicleData_request : public ALRPCRequest
{
public:

  UnsubscribeVehicleData_request(const UnsubscribeVehicleData_request& c);
  UnsubscribeVehicleData_request(void);
  
  virtual ~UnsubscribeVehicleData_request(void);

  UnsubscribeVehicleData_request& operator =(const UnsubscribeVehicleData_request&);

  bool checkIntegrity(void);

  const std::vector<VehicleDataType>* get_dataType(void) const;

  void reset_dataType(void);
  bool set_dataType(const std::vector<VehicleDataType>& dataType_);

private:

  friend class UnsubscribeVehicleData_requestMarshaller;

  std::vector<VehicleDataType>* dataType;	//!<   [%s..%s] 
};

#endif
