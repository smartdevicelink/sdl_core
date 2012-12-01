#ifndef NSAPPLINKRPC_SLIDER_RESPONSE_INCLUDE
#define NSAPPLINKRPC_SLIDER_RESPONSE_INCLUDE


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

  class Slider_response : public ALRPCResponse
  {
  public:
  
    Slider_response(const Slider_response& c);
    Slider_response(void);
    
    virtual ~Slider_response(void);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result_v2& get_resultCode(void) const;
    unsigned int get_sliderPosition(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result_v2& resultCode_);
    bool set_sliderPosition(unsigned int sliderPosition_);

  private:
  
    friend class Slider_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      Result_v2 resultCode;

///  Current slider value returned when saved or canceled
      unsigned int sliderPosition;	//!<  (1,26)
  };

}

#endif
