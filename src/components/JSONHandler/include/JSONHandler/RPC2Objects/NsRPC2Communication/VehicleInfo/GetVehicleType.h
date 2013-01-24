#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLETYPE_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLETYPE_INCLUDE

#include "JSONHandler/RPC2Request.h"


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

    class GetVehicleType : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      GetVehicleType(const GetVehicleType& c);
      GetVehicleType(void);
    
      GetVehicleType& operator =(const GetVehicleType&);
    
      virtual ~GetVehicleType(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class GetVehicleTypeMarshaller;


    };
  }
}

#endif
