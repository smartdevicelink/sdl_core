#ifndef SLIDER_RESPONSE_INCLUDE
#define SLIDER_RESPONSE_INCLUDE


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


class Slider_response : public ALRPCResponse
{
public:

  Slider_response(const Slider_response& c);
  Slider_response(void);
  
  virtual ~Slider_response(void);

  bool checkIntegrity(void);

  bool get_success(void) const;
  const Result& get_resultCode(void) const;
  unsigned int get_sliderPosition(void) const;

  bool set_success(bool success_);
  bool set_resultCode(const Result& resultCode_);
  bool set_sliderPosition(unsigned int sliderPosition_);

private:

  friend class Slider_responseMarshaller;

  bool success;
  Result resultCode;
  unsigned int sliderPosition;	//!<  (1,26)
};

#endif
