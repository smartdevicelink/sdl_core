#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_SUBSCRIBEVEHICLEDATA_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_SUBSCRIBEVEHICLEDATA_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/V2/VehicleDataType.h"

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

    class SubscribeVehicleData : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      SubscribeVehicleData(const SubscribeVehicleData& c);
      SubscribeVehicleData(void);
    
      SubscribeVehicleData& operator =(const SubscribeVehicleData&);
    
      virtual ~SubscribeVehicleData(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< NsAppLinkRPCV2::VehicleDataType>& get_dataType(void);

      int get_appId(void);


// setters
/// 1 <= size <= 100
      bool set_dataType(const std::vector< NsAppLinkRPCV2::VehicleDataType>& dataType);

      bool set_appId(int appId);


    private:

      friend class SubscribeVehicleDataMarshaller;

      std::vector< NsAppLinkRPCV2::VehicleDataType> dataType;
      int appId;

    };
  }
}

#endif
