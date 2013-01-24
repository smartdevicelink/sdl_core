#ifndef NSRPC2COMMUNICATION_APPLINKCORE_SENDDATA_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_SENDDATA_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
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

      const std::string* get_url(void);
      const int* get_timeout(void);

// setters
/// 1 <= size <= 100
      bool set_data(const std::vector< std::string>& data);

/// url <= 1000
      bool set_url(const std::string& url);

      void reset_url(void);

/// -2000000000 <= timeout <= 2000000000
      bool set_timeout(const int& timeout);

      void reset_timeout(void);


    private:

      friend class SendDataMarshaller;

      std::vector< std::string> data;
      std::string* url;
      int* timeout;

    };
  }
}

#endif
