#ifndef ALERT_RESPONSE_INCLUDE
#define ALERT_RESPONSE_INCLUDE

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


class Alert_response : public ALRPCResponse
{
public:

  Alert_response(const Alert_response& c);
  Alert_response(void);
  
  virtual ~Alert_response(void);

  Alert_response& operator =(const Alert_response&);

  bool checkIntegrity(void);

  bool get_success(void) const;
  const Result& get_resultCode(void) const;
  const std::string* get_info(void) const;
  unsigned int get_tryAgainTime(void) const;

  bool set_success(bool success_);
  bool set_resultCode(const Result& resultCode_);
  void reset_info(void);
  bool set_info(const std::string& info_);
  bool set_tryAgainTime(unsigned int tryAgainTime_);

private:

  friend class Alert_responseMarshaller;

  bool success;
  Result resultCode;
  std::string* info;	//!< (1000)
  unsigned int tryAgainTime;	//!<  (0,2000000000)
};

#endif
