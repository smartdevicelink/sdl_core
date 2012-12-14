#ifndef NSAPPLINKRPCV2_SUBSCRIBEVEHICLEDATA_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_SUBSCRIBEVEHICLEDATA_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "Result.h"
#include "VehicleDataResult.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  class SubscribeVehicleData_response : public NsAppLinkRPC::ALRPCMessage
  {
  public:
  
    SubscribeVehicleData_response(const SubscribeVehicleData_response& c);
    SubscribeVehicleData_response(void);
    
    virtual ~SubscribeVehicleData_response(void);
  
    SubscribeVehicleData_response& operator =(const SubscribeVehicleData_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result& get_resultCode(void) const;
    const std::string* get_info(void) const;
    const std::vector<VehicleDataResult>* get_dataResult(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);
    void reset_dataResult(void);
    bool set_dataResult(const std::vector<VehicleDataResult>& dataResult_);

  private:
  
    friend class SubscribeVehicleData_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      Result resultCode;

///  Provides additional human readable info regarding the result.
      std::string* info;	//!< (1000)

///  Permission status of vehicle data element.
      std::vector<VehicleDataResult>* dataResult;	//!<   [%s..%s] 
  };

}

#endif
