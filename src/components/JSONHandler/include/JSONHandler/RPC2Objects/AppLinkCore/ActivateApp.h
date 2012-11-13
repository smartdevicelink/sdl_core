#ifndef RPC2COMMUNICATION_APPLINKCORE_ACTIVATEAPP_INCLUDE
#define RPC2COMMUNICATION_APPLINKCORE_ACTIVATEAPP_INCLUDE

#include <string>
#include "JSONHandler/RPC2Request.h"


/*
  interface	RPC2Communication::AppLinkCore
  version	1.2
  generated at	Wed Nov  7 11:25:43 2012
  source stamp	Wed Nov  7 09:35:35 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace AppLinkCore
  {

    class ActivateApp : public ::RPC2Communication::RPC2Request
    {
    public:
    
      ActivateApp(const ActivateApp& c);
      ActivateApp(void);
    
      ActivateApp& operator =(const ActivateApp&);
    
      virtual ~ActivateApp(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string& get_appName(void);


// setters
/// appName <= 100
      bool set_appName(const std::string& appName);


    private:

      friend class ActivateAppMarshaller;

      std::string appName;

    };
  }
}

#endif
