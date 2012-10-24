#ifndef UNREGISTERAPPINTERFACE_RESPONSE_INCLUDE
#define UNREGISTERAPPINTERFACE_RESPONSE_INCLUDE

#include <string>

#include "Result.h"
#include "JSONHandler/ALRPCResponse.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


class UnregisterAppInterface_response : public ALRPCResponse
{
public:

  UnregisterAppInterface_response(const UnregisterAppInterface_response& c);
  UnregisterAppInterface_response(void);
  
  virtual ~UnregisterAppInterface_response(void);

  UnregisterAppInterface_response& operator =(const UnregisterAppInterface_response&);

  bool checkIntegrity(void);

  bool get_success(void) const;
  const Result& get_resultCode(void) const;
  const std::string* get_info(void) const;

  bool set_success(bool success_);
  bool set_resultCode(const Result& resultCode_);
  void reset_info(void);
  bool set_info(const std::string& info_);

private:

  friend class UnregisterAppInterface_responseMarshaller;

  bool success;
  Result resultCode;
  std::string* info;	//!< (1000)
};

#endif
