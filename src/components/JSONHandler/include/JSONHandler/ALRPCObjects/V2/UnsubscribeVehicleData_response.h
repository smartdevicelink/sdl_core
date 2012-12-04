#ifndef NSAPPLINKRPCV2_UNSUBSCRIBEVEHICLEDATA_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_UNSUBSCRIBEVEHICLEDATA_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "Result.h"
#include "VehicleDataResult.h"
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

  class UnsubscribeVehicleData_response : public ALRPC2Message
  {
  public:
  
    UnsubscribeVehicleData_response(const UnsubscribeVehicleData_response& c);
    UnsubscribeVehicleData_response(void);
    
    virtual ~UnsubscribeVehicleData_response(void);
  
    UnsubscribeVehicleData_response& operator =(const UnsubscribeVehicleData_response&);
  
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
  
    friend class UnsubscribeVehicleData_responseMarshaller;


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
