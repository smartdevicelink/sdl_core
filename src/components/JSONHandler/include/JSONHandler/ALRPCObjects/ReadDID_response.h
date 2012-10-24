#ifndef READDID_RESPONSE_INCLUDE
#define READDID_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "Result.h"
#include "VehicleDataResultCode.h"
#include "../../../JSONHandler/ALRPCResponse.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


class ReadDID_response : public ALRPCResponse
{
public:

  ReadDID_response(const ReadDID_response& c);
  ReadDID_response(void);
  
  virtual ~ReadDID_response(void);

  ReadDID_response& operator =(const ReadDID_response&);

  bool checkIntegrity(void);

  bool get_success(void) const;
  const std::vector<Result>& get_resultCode(void) const;
  const std::string* get_info(void) const;
  const std::vector<VehicleDataResultCode>* get_dataResult(void) const;
  const std::vector<std::string>* get_data(void) const;

  bool set_success(bool success_);
  bool set_resultCode(const std::vector<Result>& resultCode_);
  void reset_info(void);
  bool set_info(const std::string& info_);
  void reset_dataResult(void);
  bool set_dataResult(const std::vector<VehicleDataResultCode>& dataResult_);
  void reset_data(void);
  bool set_data(const std::vector<std::string>& data_);

private:

  friend class ReadDID_responseMarshaller;

  bool success;
  std::vector<Result> resultCode;	//!<   [%s..%s] 
  std::string* info;	//!< (1000)
  std::vector<VehicleDataResultCode>* dataResult;	//!<   [%s..%s] 
  std::vector<std::string>* data;	//!<   [%s..%s] (5000)
};

#endif
