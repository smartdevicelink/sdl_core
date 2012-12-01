#ifndef NSAPPLINKRPC_GENERICRESPONSE_V2_RESPONSE_INCLUDE
#define NSAPPLINKRPC_GENERICRESPONSE_V2_RESPONSE_INCLUDE

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

/**
     Generic Response is sent, when the name of a received msg cannot be retrieved. Only used in case of an error.
     Currently, only resultCode INVALID_DATA is used.
*/

  class GenericResponse_v2_response : public ALRPCResponse
  {
  public:
  
    GenericResponse_v2_response(const GenericResponse_v2_response& c);
    GenericResponse_v2_response(void);
    
    virtual ~GenericResponse_v2_response(void);
  
    GenericResponse_v2_response& operator =(const GenericResponse_v2_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result_v2& get_resultCode(void) const;
    const std::string* get_info(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result_v2& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);

  private:
  
    friend class GenericResponse_v2_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      Result_v2 resultCode;

///  Provides additional human readable info regarding the result.
      std::string* info;	//!< (1000)
  };

}

#endif
