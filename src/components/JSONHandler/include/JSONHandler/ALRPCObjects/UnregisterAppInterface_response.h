#ifndef APPLINKRPC_UNREGISTERAPPINTERFACE_RESPONSE_INCLUDE
#define APPLINKRPC_UNREGISTERAPPINTERFACE_RESPONSE_INCLUDE

#include <string>

#include "Result.h"
#include "JSONHandler/ALRPCResponse.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

namespace AppLinkRPC
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
