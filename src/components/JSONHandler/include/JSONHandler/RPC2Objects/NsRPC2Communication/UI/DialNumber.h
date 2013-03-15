//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_DIALNUMBER_INCLUDE
#define NSRPC2COMMUNICATION_UI_DIALNUMBER_INCLUDE

#include <string>
#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class DialNumber : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      DialNumber(const DialNumber& c);
      DialNumber(void);
    
      DialNumber& operator =(const DialNumber&);
    
      virtual ~DialNumber(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string& get_number(void);

      int get_appId(void);


// setters
/// number <= 500
      bool set_number(const std::string& number);

      bool set_appId(int appId);


    private:

      friend class DialNumberMarshaller;

      std::string number;
      int appId;

    };
  }
}

#endif
