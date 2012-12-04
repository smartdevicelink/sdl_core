#ifndef NSAPPLINKRPCV2_GETVEHICLEDATA_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_GETVEHICLEDATA_REQUEST_INCLUDE


#include "VehicleDataType.h"
#include "JSONHandler/ALRPC2Message.hh"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Non periodic vehicle data read request.

  class GetVehicleData_request : public ALRPC2Message
  {
  public:
  
    GetVehicleData_request(const GetVehicleData_request& c);
    GetVehicleData_request(void);
    
    virtual ~GetVehicleData_request(void);
  
    bool checkIntegrity(void);

    const VehicleDataType& get_dataType(void) const;

    bool set_dataType(const VehicleDataType& dataType_);

  private:
  
    friend class GetVehicleData_requestMarshaller;


///  Complex signals like "GPS" should not be transferred as separate notifications, but all together in one notification.
      VehicleDataType dataType;
  };

}

#endif
