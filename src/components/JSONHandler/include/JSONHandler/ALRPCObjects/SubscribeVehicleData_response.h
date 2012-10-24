#ifndef SUBSCRIBEVEHICLEDATA_RESPONSE_INCLUDE
#define SUBSCRIBEVEHICLEDATA_RESPONSE_INCLUDE

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


class SubscribeVehicleData_response : public ALRPCResponse
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

  bool success;
  Result resultCode;
  std::string* info;	//!< (1000)
  std::vector<VehicleDataResult>* dataResult;	//!<   [%s..%s] 
};

#endif
