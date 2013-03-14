#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLEDATA_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLEDATA_INCLUDE

#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/VehicleDataType.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
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
      const NsSmartDeviceLinkRPCV2::VehicleDataType& get_dataType(void);

      int get_appId(void);


// setters
      bool set_dataType(const NsSmartDeviceLinkRPCV2::VehicleDataType& dataType);

      bool set_appId(int appId);


    private:

      friend class GetVehicleDataMarshaller;

      NsSmartDeviceLinkRPCV2::VehicleDataType dataType;
      int appId;

    };
  }
}

#endif
