#ifndef NSAPPLINKRPCV2_DIALNUMBER_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_DIALNUMBER_REQUEST_INCLUDE

#include <string>

#include "JSONHandler/ALRPC2Message.hh"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Dials a phone number and switches to phone application.

  class DialNumber_request : public ALRPC2Message
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
