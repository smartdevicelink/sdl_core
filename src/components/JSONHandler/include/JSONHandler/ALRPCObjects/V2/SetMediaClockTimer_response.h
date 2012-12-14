#ifndef NSAPPLINKRPCV2_SETMEDIACLOCKTIMER_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_SETMEDIACLOCKTIMER_RESPONSE_INCLUDE

#include <string>

#include "Result.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  class SetMediaClockTimer_response : public NsAppLinkRPC::ALRPCMessage
  {
  public:
  
    SetMediaClockTimer_response(const SetMediaClockTimer_response& c);
    SetMediaClockTimer_response(void);
    
    virtual ~SetMediaClockTimer_response(void);
  
    SetMediaClockTimer_response& operator =(const SetMediaClockTimer_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result& get_resultCode(void) const;
    const std::string* get_info(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);

  private:
  
    friend class SetMediaClockTimer_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      Result resultCode;

///  Provides additional human readable info regarding the result.
      std::string* info;	//!< (1000)
  };

}

#endif
