#ifndef DELETEINTERACTIONCHOICESET_RESPONSE_INCLUDE
#define DELETEINTERACTIONCHOICESET_RESPONSE_INCLUDE

#include <string>

#include "Result.h"
#include "JSONHandler/ALRPCResponse.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


class DeleteInteractionChoiceSet_response : public ALRPCResponse
{
public:

  DeleteInteractionChoiceSet_response(const DeleteInteractionChoiceSet_response& c);
  DeleteInteractionChoiceSet_response(void);
  
  virtual ~DeleteInteractionChoiceSet_response(void);

  DeleteInteractionChoiceSet_response& operator =(const DeleteInteractionChoiceSet_response&);

  bool checkIntegrity(void);

  bool get_success(void) const;
  const Result& get_resultCode(void) const;
  const std::string* get_info(void) const;

  bool set_success(bool success_);
  bool set_resultCode(const Result& resultCode_);
  void reset_info(void);
  bool set_info(const std::string& info_);

private:

  friend class DeleteInteractionChoiceSet_responseMarshaller;

  bool success;
  Result resultCode;
  std::string* info;	//!< (1000)
};

#endif
