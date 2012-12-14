#ifndef NSAPPLINKRPC_HMIAPPLICATION_INCLUDE
#define NSAPPLINKRPC_HMIAPPLICATION_INCLUDE

#include <string>



/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
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

    const std::string& get_appName(void) const;
    const std::string* get_icon(void) const;
    const std::string* get_ngnMediaScreenAppName(void) const;

// setters

    bool set_appName(const std::string& appName_);
    void reset_icon(void);
    bool set_icon(const std::string& icon_);
    void reset_ngnMediaScreenAppName(void);
    bool set_ngnMediaScreenAppName(const std::string& ngnMediaScreenAppName_);

  private:

    friend class HMIApplicationMarshaller;

      std::string appName;	//!< (100)
      std::string* icon;
      std::string* ngnMediaScreenAppName;	//!< (100)
  };

}

#endif
