#ifndef SUBSCRIBEBUTTON_RESPONSE_INCLUDE
#define SUBSCRIBEBUTTON_RESPONSE_INCLUDE

#include <string>

#include "Result.h"
#include "JSONHandler/ALRPCResponse.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


class SubscribeButton_response : public ALRPCResponse
{
public:

  SubscribeButton_response(const SubscribeButton_response& c);
  SubscribeButton_response(void);
  
  virtual ~SubscribeButton_response(void);

  SubscribeButton_response& operator =(const SubscribeButton_response&);

  bool checkIntegrity(void);

  bool get_success(void) const;
  const Result& get_resultCode(void) const;
  const std::string* get_info(void) const;

  bool set_success(bool success_);
  bool set_resultCode(const Result& resultCode_);
  void reset_info(void);
  bool set_info(const std::string& info_);

private:

  friend class SubscribeButton_responseMarshaller;

  bool success;
  Result resultCode;
  std::string* info;	//!< (1000)
};

#endif
