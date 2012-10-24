#ifndef DIALNUMBER_REQUEST_INCLUDE
#define DIALNUMBER_REQUEST_INCLUDE

#include <string>

#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


///  Dials a phone number and switches to phone application.

class DialNumber_request : public ALRPCRequest
{
public:

  DialNumber_request(const DialNumber_request& c);
  DialNumber_request(void);
  
  virtual ~DialNumber_request(void);

  bool checkIntegrity(void);

  const std::string& get_number(void) const;

  bool set_number(const std::string& number_);

private:

  friend class DialNumber_requestMarshaller;

  std::string number;	//!< (500)
};

#endif
