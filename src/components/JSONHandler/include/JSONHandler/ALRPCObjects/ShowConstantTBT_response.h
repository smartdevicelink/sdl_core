#ifndef SHOWCONSTANTTBT_RESPONSE_INCLUDE
#define SHOWCONSTANTTBT_RESPONSE_INCLUDE

#include <string>

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


class ShowConstantTBT_response : public ALRPCResponse
{
public:

  ShowConstantTBT_response(const ShowConstantTBT_response& c);
  ShowConstantTBT_response(void);
  
  virtual ~ShowConstantTBT_response(void);

  ShowConstantTBT_response& operator =(const ShowConstantTBT_response&);

  bool checkIntegrity(void);

  bool get_success(void) const;
  const Result& get_resultCode(void) const;
  const std::string* get_info(void) const;

  bool set_success(bool success_);
  bool set_resultCode(const Result& resultCode_);
  void reset_info(void);
  bool set_info(const std::string& info_);

private:

  friend class ShowConstantTBT_responseMarshaller;

  bool success;
  Result resultCode;
  std::string* info;	//!< (1000)
};

#endif
