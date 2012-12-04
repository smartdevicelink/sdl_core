#ifndef NSAPPLINKRPCV2_ALERT_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_ALERT_RESPONSE_INCLUDE

#include <string>

#include "Result.h"
#include "JSONHandler/ALRPC2Message.hh"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  class Alert_response : public ALRPC2Message
  {
  public:
  
    Alert_response(const Alert_response& c);
    Alert_response(void);
    
    virtual ~Alert_response(void);
  
    Alert_response& operator =(const Alert_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result& get_resultCode(void) const;
    const std::string* get_info(void) const;
    unsigned int get_tryAgainTime(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);
    bool set_tryAgainTime(unsigned int tryAgainTime_);

  private:
  
    friend class Alert_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      Result resultCode;

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
