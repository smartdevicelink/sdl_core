//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_SUBSCRIBEVEHICLEDATA_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SUBSCRIBEVEHICLEDATA_REQUEST_INCLUDE

#include <vector>

#include "VehicleDataType.h"
#include "JSONHandler/SDLRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{

/**
     Subscribes for specific published data items.
     The data will be only sent if it has changed.
     The application will be notified by the onVehicleData notification whenever new data is available.
     To unsubscribe the notifications, use unsubscribe with the same subscriptionType.
*/

  class SubscribeVehicleData_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    SubscribeVehicleData_request(const SubscribeVehicleData_request& c);
    SubscribeVehicleData_request(void);
    
    virtual ~SubscribeVehicleData_request(void);
  
    bool checkIntegrity(void);

    const std::vector<VehicleDataType>& get_dataType(void) const;

    bool set_dataType(const std::vector<VehicleDataType>& dataType_);

  private:
  
    friend class SubscribeVehicleData_requestMarshaller;


/**
     List of defined vehicle data elements.
     See VehicleDataType
*/
      std::vector<VehicleDataType> dataType;	//!<   [%s..%s] 
  };

}

#endif
