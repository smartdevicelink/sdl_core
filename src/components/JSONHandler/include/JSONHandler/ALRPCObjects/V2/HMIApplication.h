#ifndef NSAPPLINKRPCV2_HMIAPPLICATION_INCLUDE
#define NSAPPLINKRPCV2_HMIAPPLICATION_INCLUDE

#include <string>
#include <vector>

#include "AppType.h"
#include "Language.h"
#include "Language.h"
#include "TTSChunk.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/


namespace NsAppLinkRPCV2
{

///  Data type containing information about application needed by HMI.

  class HMIApplication
  {
  public:
  
    HMIApplication(const HMIApplication& c);
    HMIApplication(void);
  
    bool checkIntegrity(void);
  
    ~HMIApplication(void);
    HMIApplication& operator =(const HMIApplication&);

// getters

    int get_appId(void) const;
    const std::string& get_appName(void) const;
    const std::vector<AppType>* get_appType(void) const;
    const std::string& get_deviceName(void) const;
    const Language& get_hmiDisplayLanguageDesired(void) const;
    const std::string* get_icon(void) const;
    bool get_isMediaApplication(void) const;
    const Language& get_languageDesired(void) const;
    const std::string* get_ngnMediaScreenAppName(void) const;
    const std::vector<TTSChunk>* get_ttsName(void) const;
    const std::vector<std::string>* get_vrSynonyms(void) const;

// setters

    bool set_appId(int appId_);
    bool set_appName(const std::string& appName_);
    void reset_appType(void);
    bool set_appType(const std::vector<AppType>& appType_);
    bool set_deviceName(const std::string& deviceName_);
    bool set_hmiDisplayLanguageDesired(const Language& hmiDisplayLanguageDesired_);
    void reset_icon(void);
    bool set_icon(const std::string& icon_);
    bool set_isMediaApplication(bool isMediaApplication_);
    bool set_languageDesired(const Language& languageDesired_);
    void reset_ngnMediaScreenAppName(void);
    bool set_ngnMediaScreenAppName(const std::string& ngnMediaScreenAppName_);
    void reset_ttsName(void);
    bool set_ttsName(const std::vector<TTSChunk>& ttsName_);
    void reset_vrSynonyms(void);
    bool set_vrSynonyms(const std::vector<std::string>& vrSynonyms_);

  private:

    friend class HMIApplicationMarshaller;

      int appId;
      std::string appName;	//!< (100)
      std::vector<AppType>* appType;	//!<   [%s..%s] 
      std::string deviceName;
      Language hmiDisplayLanguageDesired;
      std::string* icon;
      bool isMediaApplication;
      Language languageDesired;
      std::string* ngnMediaScreenAppName;	//!< (100)
      std::vector<TTSChunk>* ttsName;	//!<   [%s..%s] 
      std::vector<std::string>* vrSynonyms;	//!<   [%s..%s] (40)
  };

}

#endif
