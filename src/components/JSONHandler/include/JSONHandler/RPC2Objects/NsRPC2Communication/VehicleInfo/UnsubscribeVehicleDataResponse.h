#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_UNSUBSCRIBEVEHICLEDATARESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_UNSUBSCRIBEVEHICLEDATARESPONSE_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/ALRPCObjects/V2/VehicleDataResult.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    class UnsubscribeVehicleDataResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      UnsubscribeVehicleDataResponse(const UnsubscribeVehicleDataResponse& c);
      UnsubscribeVehicleDataResponse(void);
    
      UnsubscribeVehicleDataResponse& operator =(const UnsubscribeVehicleDataResponse&);
    
      virtual ~UnsubscribeVehicleDataResponse(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< NsAppLinkRPCV2::VehicleDataResult>* get_dataResult(void);

// setters
/// 1 <= size <= 100
      bool set_dataResult(const std::vector< NsAppLinkRPCV2::VehicleDataResult>& dataResult);

      void reset_dataResult(void);


    private:

      friend class UnsubscribeVehicleDataResponseMarshaller;

      std::vector< NsAppLinkRPCV2::VehicleDataResult>* dataResult;

    };
  }
}

#endif
