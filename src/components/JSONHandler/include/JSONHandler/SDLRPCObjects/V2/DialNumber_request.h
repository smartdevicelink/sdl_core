//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_DIALNUMBER_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_DIALNUMBER_REQUEST_INCLUDE

#include <string>

#include "JSONHandler/SDLRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{

///  Dials a phone number and switches to phone application.

  class DialNumber_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
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
