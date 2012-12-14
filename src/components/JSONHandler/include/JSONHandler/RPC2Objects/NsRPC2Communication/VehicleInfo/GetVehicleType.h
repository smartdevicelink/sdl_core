#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLETYPE_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLETYPE_INCLUDE

#include "JSONHandler/RPC2Request.h"


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

    class GetVehicleType : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      GetVehicleType(const GetVehicleType& c);
      GetVehicleType(void);
    
      GetVehicleType& operator =(const GetVehicleType&);
    
      virtual ~GetVehicleType(void);
    
      bool checkIntegrity(void);
    
// getters
      int get_appId(void);


// setters
      bool set_appId(int appId);


    private:

      friend class GetVehicleTypeMarshaller;

      int appId;

    };
  }
}

#endif
