#ifndef NSRPC2COMMUNICATION_PHONE_DIALNUMBER_INCLUDE
#define NSRPC2COMMUNICATION_PHONE_DIALNUMBER_INCLUDE

#include <string>
#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::Phone
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace Phone
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
