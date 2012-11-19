#ifndef NSRPC2COMMUNICATION_APPLINKCORE_SENDDATA_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_SENDDATA_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Mon Nov 19 12:18:27 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace AppLinkCore
  {

    class SendData : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      SendData(const SendData& c);
      SendData(void);
    
      SendData& operator =(const SendData&);
    
      virtual ~SendData(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< std::string>& get_data(void);


// setters
/// 1 <= size <= 100
      bool set_data(const std::vector< std::string>& data);


    private:

      friend class SendDataMarshaller;

      std::vector< std::string> data;

    };
  }
}

#endif
