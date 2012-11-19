#ifndef NSAPPLINKRPC_RESETGLOBALPROPERTIES_RESPONSE_INCLUDE
#define NSAPPLINKRPC_RESETGLOBALPROPERTIES_RESPONSE_INCLUDE

#include <string>

#include "Result.h"
#include "JSONHandler/ALRPCResponse.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 10:37:06 2012
  source stamp	Mon Nov 19 10:35:56 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  class ResetGlobalProperties_response : public ALRPCResponse
  {
  public:
  
    ResetGlobalProperties_response(const ResetGlobalProperties_response& c);
    ResetGlobalProperties_response(void);
    
    virtual ~ResetGlobalProperties_response(void);
  
    ResetGlobalProperties_response& operator =(const ResetGlobalProperties_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result& get_resultCode(void) const;
    const std::string* get_info(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);

  private:
  
    friend class ResetGlobalProperties_responseMarshaller;


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
