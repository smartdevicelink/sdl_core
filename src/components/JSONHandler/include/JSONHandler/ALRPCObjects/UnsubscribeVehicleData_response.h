#ifndef UNSUBSCRIBEVEHICLEDATA_RESPONSE_INCLUDE
#define UNSUBSCRIBEVEHICLEDATA_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "Result.h"
#include "VehicleDataResult.h"
#include "../../../JSONHandler/ALRPCResponse.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


class UnsubscribeVehicleData_response : public ALRPCResponse
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

  bool success;
  Result resultCode;
  std::string* info;	//!< (1000)
  std::vector<VehicleDataResult>* dataResult;	//!<   [%s..%s] 
};

#endif
