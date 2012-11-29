#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLETYPE_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLETYPE_INCLUDE

#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
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
