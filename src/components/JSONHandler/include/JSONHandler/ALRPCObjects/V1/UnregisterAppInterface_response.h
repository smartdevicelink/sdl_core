#ifndef NSAPPLINKRPC_UNREGISTERAPPINTERFACE_RESPONSE_INCLUDE
#define NSAPPLINKRPC_UNREGISTERAPPINTERFACE_RESPONSE_INCLUDE

#include <string>

#include "Result.h"
#include "JSONHandler/ALRPCResponse.h"


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

  class UnregisterAppInterface_response : public ALRPCResponse
  {
  public:
  
    UnregisterAppInterface_response(const UnregisterAppInterface_response& c);
    UnregisterAppInterface_response(void);
    
    virtual ~UnregisterAppInterface_response(void);
  
    UnregisterAppInterface_response& operator =(const UnregisterAppInterface_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result& get_resultCode(void) const;
    const std::string* get_info(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);

  private:
  
    friend class UnregisterAppInterface_responseMarshaller;


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
