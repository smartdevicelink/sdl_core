#ifndef NSAPPLINKRPC_DIALNUMBER_REQUEST_INCLUDE
#define NSAPPLINKRPC_DIALNUMBER_REQUEST_INCLUDE

#include <string>

#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

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


///  Phone number is a string consisting of only the digits and "+", which can be up to 40 chars.
      std::string number;	//!< (500)
  };

}

#endif
