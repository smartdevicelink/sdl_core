#ifndef SCROLLABLEMESSAGE_RESPONSE_INCLUDE
#define SCROLLABLEMESSAGE_RESPONSE_INCLUDE


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


class ScrollableMessage_response : public ALRPCResponse
{
public:

  ScrollableMessage_response(const ScrollableMessage_response& c);
  ScrollableMessage_response(void);
  
  virtual ~ScrollableMessage_response(void);

  bool checkIntegrity(void);

  bool get_success(void) const;
  const Result& get_resultCode(void) const;

  bool set_success(bool success_);
  bool set_resultCode(const Result& resultCode_);

private:

  friend class ScrollableMessage_responseMarshaller;

  bool success;
  Result resultCode;
};

#endif
