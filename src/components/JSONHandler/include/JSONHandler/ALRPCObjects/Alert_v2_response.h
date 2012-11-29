#ifndef NSAPPLINKRPC_ALERT_V2_RESPONSE_INCLUDE
#define NSAPPLINKRPC_ALERT_V2_RESPONSE_INCLUDE

#include <string>

#include "Result_v2.h"
#include "JSONHandler/ALRPCResponse.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  class Alert_v2_response : public ALRPCResponse
  {
  public:
  
    Alert_v2_response(const Alert_v2_response& c);
    Alert_v2_response(void);
    
    virtual ~Alert_v2_response(void);
  
    Alert_v2_response& operator =(const Alert_v2_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result_v2& get_resultCode(void) const;
    const std::string* get_info(void) const;
    unsigned int get_tryAgainTime(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result_v2& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);
    bool set_tryAgainTime(unsigned int tryAgainTime_);

  private:
  
    friend class Alert_v2_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      Result_v2 resultCode;

///  Provides additional human readable info regarding the result.
      std::string* info;	//!< (1000)

/**
     Amount of time (in seconds) that an app must wait before resending an alert.
     If provided, another system event or overlay currently has a higher priority than this alert.
     An app must not send an alert without waiting at least the amount of time dictated.
*/
      unsigned int tryAgainTime;	//!<  (0,2000000000)
  };

}

#endif
