#ifndef SETAPPICON_RESPONSE_INCLUDE
#define SETAPPICON_RESPONSE_INCLUDE

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
     Response is sent, when the file data was copied (success case). Or when an error occured.
     Not supported on First generation SYNC vehicles. 
     Response is sent, when the file data was copied (success case). Or when an error occured.
     Not supported on First generation SYNC vehicles. 
*/

class SetAppIcon_response : public ALRPCResponse
{
public:

  SetAppIcon_response(const SetAppIcon_response& c);
  SetAppIcon_response(void);
  
  virtual ~SetAppIcon_response(void);

  SetAppIcon_response& operator =(const SetAppIcon_response&);

  bool checkIntegrity(void);

  bool get_success(void) const;
  const Result& get_resultCode(void) const;
  const std::string* get_info(void) const;

  bool set_success(bool success_);
  bool set_resultCode(const Result& resultCode_);
  void reset_info(void);
  bool set_info(const std::string& info_);

private:

  friend class SetAppIcon_responseMarshaller;

  bool success;
  Result resultCode;
  std::string* info;	//!< (1000)
};

#endif
