#ifndef NSRPC2COMMUNICATION_APPLINKCORE_ONAPPREGISTERED_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_ONAPPREGISTERED_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/Language.h"
#include "../include/JSONHandler/ALRPCObjects/Language.h"
#include "../include/JSONHandler/ALRPCObjects/TTSChunk.h"
#include "../include/JSONHandler/ALRPCObjects/AppType.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
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

      const NsAppLinkRPC::Language& get_hmiDisplayLanguageDesired(void);

      const std::vector< NsAppLinkRPC::TTSChunk>* get_ttsName(void);
      const std::vector< NsAppLinkRPC::AppType>* get_appType(void);
      int get_appId(void);


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

      bool set_hmiDisplayLanguageDesired(const NsAppLinkRPC::Language& hmiDisplayLanguageDesired);

/// 1 <= size <= 100
      bool set_ttsName(const std::vector< NsAppLinkRPC::TTSChunk>& ttsName);

      void reset_ttsName(void);

/// 1 <= size <= 100
      bool set_appType(const std::vector< NsAppLinkRPC::AppType>& appType);

      void reset_appType(void);

      bool set_appId(int appId);


    private:

      friend class OnAppRegisteredMarshaller;

      std::string appName;
      std::string appIcon;
      std::string deviceName;
      std::vector< std::string>* vrSynonym;
      bool isMediaApplication;
      NsAppLinkRPC::Language languageDesired;
      NsAppLinkRPC::Language hmiDisplayLanguageDesired;
      std::vector< NsAppLinkRPC::TTSChunk>* ttsName;
      std::vector< NsAppLinkRPC::AppType>* appType;
      int appId;

    };
  }
}

#endif
