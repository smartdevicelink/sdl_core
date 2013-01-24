#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLETYPERESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLETYPERESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/ALRPCObjects/V2/VehicleType.h"

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

    class GetVehicleTypeResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      GetVehicleTypeResponse(const GetVehicleTypeResponse& c);
      GetVehicleTypeResponse(void);
    
      GetVehicleTypeResponse& operator =(const GetVehicleTypeResponse&);
    
      virtual ~GetVehicleTypeResponse(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsAppLinkRPCV2::VehicleType& get_vehicleType(void);


// setters
      bool set_vehicleType(const NsAppLinkRPCV2::VehicleType& vehicleType);


    private:

      friend class GetVehicleTypeResponseMarshaller;

      NsAppLinkRPCV2::VehicleType vehicleType;

    };
  }
}

#endif
