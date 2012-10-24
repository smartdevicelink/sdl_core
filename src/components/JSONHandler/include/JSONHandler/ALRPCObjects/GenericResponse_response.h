#ifndef GENERICRESPONSE_RESPONSE_INCLUDE
#define GENERICRESPONSE_RESPONSE_INCLUDE

#include <string>

#include "Result.h"
#include "../../../JSONHandler/ALRPCResponse.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


/**
     Generic Response is sent, when the name of a received msg cannot be retrieved. Only used in case of an error.
     Currently, only resultCode INVALID_DATA is used.
*/

class GenericResponse_response : public ALRPCResponse
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

  bool success;
  Result resultCode;
  std::string* info;	//!< (1000)
};

#endif
