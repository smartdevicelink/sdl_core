#ifndef SUBSCRIBEVEHICLEDATA_REQUEST_INCLUDE
#define SUBSCRIBEVEHICLEDATA_REQUEST_INCLUDE

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


/**
     Subscribes for specific published data items.
     The data will be only sent if it has changed.
     The application will be notified by the onVehicleData notification whenever new data is available.
     To unsubscribe the notifications, use unsubscribe with the same subscriptionType.
*/

class SubscribeVehicleData_request : public ALRPCRequest
{
public:

  SubscribeVehicleData_request(const SubscribeVehicleData_request& c);
  SubscribeVehicleData_request(void);
  
  virtual ~SubscribeVehicleData_request(void);

  SubscribeVehicleData_request& operator =(const SubscribeVehicleData_request&);

  bool checkIntegrity(void);

  const std::vector<VehicleDataType>* get_dataType(void) const;

  void reset_dataType(void);
  bool set_dataType(const std::vector<VehicleDataType>& dataType_);

private:

  friend class SubscribeVehicleData_requestMarshaller;

  std::vector<VehicleDataType>* dataType;	//!<   [%s..%s] 
};

#endif
