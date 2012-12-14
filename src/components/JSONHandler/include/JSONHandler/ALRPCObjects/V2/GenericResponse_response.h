#ifndef NSAPPLINKRPCV2_GENERICRESPONSE_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_GENERICRESPONSE_RESPONSE_INCLUDE

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

/**
     Generic Response is sent, when the name of a received msg cannot be retrieved. Only used in case of an error.
     Currently, only resultCode INVALID_DATA is used.
*/

  class GenericResponse_response : public NsAppLinkRPC::ALRPCMessage
  {
  public:
  
    GenericResponse_response(const GenericResponse_response& c);
    GenericResponse_response(void);
    
    virtual ~GenericResponse_response(void);
  
    GenericResponse_response& operator =(const GenericResponse_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result& get_resultCode(void) const;
    const std::string* get_info(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);

  private:
  
    friend class GenericResponse_responseMarshaller;


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
