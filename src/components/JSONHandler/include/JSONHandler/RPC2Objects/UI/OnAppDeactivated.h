#ifndef RPC2COMMUNICATION_UI_ONAPPDEACTIVATED_INCLUDE
#define RPC2COMMUNICATION_UI_ONAPPDEACTIVATED_INCLUDE

#include <string>
#include "JSONHandler/RPC2Notification.h"


/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace UI
  {

    class OnAppDeactivated : public ::RPC2Communication::RPC2Notification
    {
    public:
    
      OnAppDeactivated(const OnAppDeactivated& c);
      OnAppDeactivated(void);
    
      OnAppDeactivated& operator =(const OnAppDeactivated&);
    
      virtual ~OnAppDeactivated(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string& get_appName(void);


// setters
/// appName <= 100
      bool set_appName(const std::string& appName);


    private:

      friend class OnAppDeactivatedMarshaller;

      std::string appName;

    };
  }
}

#endif
