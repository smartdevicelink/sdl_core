#ifndef NSAPPLINKRPC_SUBSCRIBEVEHICLEDATA_REQUEST_INCLUDE
#define NSAPPLINKRPC_SUBSCRIBEVEHICLEDATA_REQUEST_INCLUDE

#include <vector>

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


/**
     List of defined vehicle data elements.
     See VehicleDataType
*/
      std::vector<VehicleDataType>* dataType;	//!<   [%s..%s] 
  };

}

#endif
