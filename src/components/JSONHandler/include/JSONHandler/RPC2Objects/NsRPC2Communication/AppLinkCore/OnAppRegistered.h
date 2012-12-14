#ifndef NSRPC2COMMUNICATION_APPLINKCORE_ONAPPREGISTERED_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_ONAPPREGISTERED_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/V1/Language.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Language.h"
#include "../include/JSONHandler/ALRPCObjects/V1/TTSChunk.h"
#include "../include/JSONHandler/ALRPCObjects/V2/AppType.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
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

      const NsAppLinkRPC::Language* get_hmiDisplayLanguageDesired(void);
      const std::vector< NsAppLinkRPC::TTSChunk>* get_ttsName(void);
      const std::vector< NsAppLinkRPCV2::AppType>* get_appType(void);
      unsigned int get_versionNumber(void);

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

      void reset_hmiDisplayLanguageDesired(void);

/// 1 <= size <= 100
      bool set_ttsName(const std::vector< NsAppLinkRPC::TTSChunk>& ttsName);

      void reset_ttsName(void);

/// 1 <= size <= 100
      bool set_appType(const std::vector< NsAppLinkRPCV2::AppType>& appType);

      void reset_appType(void);

      bool set_versionNumber(unsigned int versionNumber);

      bool set_appId(int appId);


    private:

      friend class OnAppRegisteredMarshaller;

      std::string appName;
      std::string appIcon;
      std::string deviceName;
      std::vector< std::string>* vrSynonym;
      bool isMediaApplication;
      NsAppLinkRPC::Language languageDesired;
      NsAppLinkRPC::Language* hmiDisplayLanguageDesired;
      std::vector< NsAppLinkRPC::TTSChunk>* ttsName;
      std::vector< NsAppLinkRPCV2::AppType>* appType;
      unsigned int versionNumber;
      int appId;

    };
  }
}

#endif
