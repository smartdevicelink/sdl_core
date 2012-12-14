#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLEDATA_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLEDATA_INCLUDE

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

    class GetVehicleData : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      GetVehicleData(const GetVehicleData& c);
      GetVehicleData(void);
    
      GetVehicleData& operator =(const GetVehicleData&);
    
      virtual ~GetVehicleData(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsAppLinkRPCV2::VehicleDataType& get_dataType(void);

      int get_appId(void);


// setters
      bool set_dataType(const NsAppLinkRPCV2::VehicleDataType& dataType);

      bool set_appId(int appId);


    private:

      friend class GetVehicleDataMarshaller;

      NsAppLinkRPCV2::VehicleDataType dataType;
      int appId;

    };
  }
}

#endif
