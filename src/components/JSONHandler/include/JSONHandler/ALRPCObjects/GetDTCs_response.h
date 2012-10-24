#ifndef GETDTCS_RESPONSE_INCLUDE
#define GETDTCS_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "DTC.h"
#include "Result.h"
#include "../../../JSONHandler/ALRPCResponse.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


class GetDTCs_response : public ALRPCResponse
{
public:

  GetDTCs_response(const GetDTCs_response& c);
  GetDTCs_response(void);
  
  virtual ~GetDTCs_response(void);

  GetDTCs_response& operator =(const GetDTCs_response&);

  bool checkIntegrity(void);

  bool get_success(void) const;
  const std::vector<Result>& get_resultCode(void) const;
  const std::string* get_info(void) const;
  const std::vector<DTC>* get_dtcList(void) const;

  bool set_success(bool success_);
  bool set_resultCode(const std::vector<Result>& resultCode_);
  void reset_info(void);
  bool set_info(const std::string& info_);
  void reset_dtcList(void);
  bool set_dtcList(const std::vector<DTC>& dtcList_);

private:

  friend class GetDTCs_responseMarshaller;

  bool success;
  std::vector<Result> resultCode;	//!<   [%s..%s] 
  std::string* info;	//!< (1000)
  std::vector<DTC>* dtcList;	//!<   [%s..%s] 
};

#endif
