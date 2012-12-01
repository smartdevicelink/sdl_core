#ifndef NSAPPLINKRPC_UNSUBSCRIBEVEHICLEDATA_RESPONSE_INCLUDE
#define NSAPPLINKRPC_UNSUBSCRIBEVEHICLEDATA_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "Result_v2.h"
#include "VehicleDataResult.h"
#include "JSONHandler/ALRPCResponse.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  class UnsubscribeVehicleData_response : public ALRPCResponse
  {
  public:
  
    UnsubscribeVehicleData_response(const UnsubscribeVehicleData_response& c);
    UnsubscribeVehicleData_response(void);
    
    virtual ~UnsubscribeVehicleData_response(void);
  
    UnsubscribeVehicleData_response& operator =(const UnsubscribeVehicleData_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result_v2& get_resultCode(void) const;
    const std::string* get_info(void) const;
    const std::vector<VehicleDataResult>* get_dataResult(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result_v2& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);
    void reset_dataResult(void);
    bool set_dataResult(const std::vector<VehicleDataResult>& dataResult_);

  private:
  
    friend class UnsubscribeVehicleData_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      Result_v2 resultCode;

///  Provides additional human readable info regarding the result.
      std::string* info;	//!< (1000)

///  Permission status of vehicle data element.
      std::vector<VehicleDataResult>* dataResult;	//!<   [%s..%s] 
  };

}

#endif
