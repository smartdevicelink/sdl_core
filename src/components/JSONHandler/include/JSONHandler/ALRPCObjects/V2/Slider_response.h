#ifndef NSAPPLINKRPCV2_SLIDER_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_SLIDER_RESPONSE_INCLUDE


#include "Result.h"
#include "JSONHandler/ALRPC2Message.hh"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  class Slider_response : public ALRPC2Message
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


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      Result resultCode;

///  Current slider value returned when saved or canceled
      unsigned int sliderPosition;	//!<  (1,26)
  };

}

#endif
