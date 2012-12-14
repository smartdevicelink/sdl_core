#ifndef NSAPPLINKRPC_HMIAPPLICATION_INCLUDE
#define NSAPPLINKRPC_HMIAPPLICATION_INCLUDE

#include <string>



/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Dec 14 08:11:37 2012
  source stamp	Fri Dec 14 08:11:34 2012
  author	robok0der
*/


namespace NsAppLinkRPC
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
    const std::string* get_icon(void) const;
    bool get_isMediaApplication(void) const;
    const std::string* get_ngnMediaScreenAppName(void) const;

// setters

    bool set_appId(int appId_);
    bool set_appName(const std::string& appName_);
    void reset_icon(void);
    bool set_icon(const std::string& icon_);
    bool set_isMediaApplication(bool isMediaApplication_);
    void reset_ngnMediaScreenAppName(void);
    bool set_ngnMediaScreenAppName(const std::string& ngnMediaScreenAppName_);

  private:

    friend class HMIApplicationMarshaller;

      int appId;
      std::string appName;	//!< (100)
      std::string* icon;
      bool isMediaApplication;
      std::string* ngnMediaScreenAppName;	//!< (100)
  };

}

#endif
