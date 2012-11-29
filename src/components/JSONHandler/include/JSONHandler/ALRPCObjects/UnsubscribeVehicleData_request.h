#ifndef NSAPPLINKRPC_UNSUBSCRIBEVEHICLEDATA_REQUEST_INCLUDE
#define NSAPPLINKRPC_UNSUBSCRIBEVEHICLEDATA_REQUEST_INCLUDE

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


/**
     Refers to the vehicle data elements specified by subscribeVehicleData.
     See VehicleDataType
*/
      std::vector<VehicleDataType>* dataType;	//!<   [%s..%s] 
  };

}

#endif
