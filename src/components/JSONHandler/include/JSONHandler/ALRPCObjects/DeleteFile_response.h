#ifndef DELETEFILE_RESPONSE_INCLUDE
#define DELETEFILE_RESPONSE_INCLUDE

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
     Response is sent, when the file data was deleted (success case). Or when an error occured.
     Not supported on First generation SYNC vehicles. 
*/

class DeleteFile_response : public ALRPCResponse
{
public:

  DeleteFile_response(const DeleteFile_response& c);
  DeleteFile_response(void);
  
  virtual ~DeleteFile_response(void);

  DeleteFile_response& operator =(const DeleteFile_response&);

  bool checkIntegrity(void);

  bool get_success(void) const;
  const Result& get_resultCode(void) const;
  unsigned int get_spaceAvailable(void) const;
  const std::string* get_info(void) const;

  bool set_success(bool success_);
  bool set_resultCode(const Result& resultCode_);
  bool set_spaceAvailable(unsigned int spaceAvailable_);
  void reset_info(void);
  bool set_info(const std::string& info_);

private:

  friend class DeleteFile_responseMarshaller;

  bool success;
  Result resultCode;
  unsigned int spaceAvailable;	//!<  (0,2000000000)
  std::string* info;	//!< (1000)
};

#endif
