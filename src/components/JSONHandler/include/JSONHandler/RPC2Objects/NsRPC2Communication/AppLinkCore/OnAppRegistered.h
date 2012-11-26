#ifndef NSRPC2COMMUNICATION_APPLINKCORE_ONAPPREGISTERED_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_ONAPPREGISTERED_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/Language.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Nov 20 13:32:23 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace AppLinkCore
  {

    class OnAppRegistered : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnAppRegistered(const OnAppRegistered& c);
      OnAppRegistered(void);
    
      OnAppRegistered& operator =(const OnAppRegistered&);
    
      virtual ~OnAppRegistered(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string& get_appName(void);

      const std::string& get_appIcon(void);

      const std::string& get_deviceName(void);

      const std::vector< std::string>* get_vrSynonym(void);
      bool get_isMediaApplication(void);

      const NsAppLinkRPC::Language& get_languageDesired(void);


// setters
/// appName <= 100
      bool set_appName(const std::string& appName);

      bool set_appIcon(const std::string& appIcon);

      bool set_deviceName(const std::string& deviceName);

/// vrSynonym[] <= 40 ; 1 <= size <= 100
      bool set_vrSynonym(const std::vector< std::string>& vrSynonym);

      void reset_vrSynonym(void);

      bool set_isMediaApplication(bool isMediaApplication);

      bool set_languageDesired(const NsAppLinkRPC::Language& languageDesired);


    private:

      friend class OnAppRegisteredMarshaller;

      std::string appName;
      std::string appIcon;
      std::string deviceName;
      std::vector< std::string>* vrSynonym;
      bool isMediaApplication;
      NsAppLinkRPC::Language languageDesired;

    };
  }
}

#endif
