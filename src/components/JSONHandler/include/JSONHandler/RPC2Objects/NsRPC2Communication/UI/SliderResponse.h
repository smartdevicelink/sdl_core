#ifndef NSRPC2COMMUNICATION_UI_SLIDERRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_SLIDERRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class SliderResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      SliderResponse(const SliderResponse& c);
      SliderResponse(void);
    
      SliderResponse& operator =(const SliderResponse&);
    
      virtual ~SliderResponse(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_sliderPosition(void);


// setters
/// 1 <= sliderPosition <= 26
      bool set_sliderPosition(unsigned int sliderPosition);


    private:

      friend class SliderResponseMarshaller;

      unsigned int sliderPosition;

    };
  }
}

#endif
