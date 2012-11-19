#ifndef NSRPC2COMMUNICATION_UI_ONAPPACTIVATED_INCLUDE
#define NSRPC2COMMUNICATION_UI_ONAPPACTIVATED_INCLUDE

#include <string>
#include "JSONHandler/RPC2Notification.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 10:37:39 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class OnAppActivated : public ::NsRPC2Communication::RPC2Notification
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
