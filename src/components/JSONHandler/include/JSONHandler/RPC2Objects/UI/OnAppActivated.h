#ifndef RPC2COMMUNICATION_UI_ONAPPACTIVATED_INCLUDE
#define RPC2COMMUNICATION_UI_ONAPPACTIVATED_INCLUDE

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

    class OnAppActivated : public ::RPC2Communication::RPC2Notification
    {
    public:
    
      OnAppActivated(const OnAppActivated& c);
      OnAppActivated(void);
    
      OnAppActivated& operator =(const OnAppActivated&);
    
      virtual ~OnAppActivated(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string& get_appName(void);


// setters
/// appName <= 100
      bool set_appName(const std::string& appName);


    private:

      friend class OnAppActivatedMarshaller;

      std::string appName;

    };
  }
}

#endif
