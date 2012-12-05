#ifndef NSRPC2COMMUNICATION_APPLINKCORE_SENDDATA_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_SENDDATA_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
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

      int get_appId(void);


// setters
/// 1 <= size <= 100
      bool set_data(const std::vector< std::string>& data);

      bool set_appId(int appId);


    private:

      friend class SendDataMarshaller;

      std::vector< std::string> data;
      int appId;

    };
  }
}

#endif
