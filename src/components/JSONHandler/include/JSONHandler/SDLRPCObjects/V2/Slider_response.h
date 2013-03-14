#ifndef NSAPPLINKRPCV2_SLIDER_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_SLIDER_RESPONSE_INCLUDE

#include "JSONHandler/SDLRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    robok0der
*/

namespace NsSmartDeviceLinkRPCV2
{
    class Slider_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        Slider_response(const Slider_response& c);
        Slider_response(void);

        virtual ~Slider_response(void);

        Slider_response& operator =(const Slider_response& c);

        bool checkIntegrity(void);

        unsigned int get_sliderPosition(void) const;
        bool set_sliderPosition(unsigned int sliderPosition_);

    private:
        friend class Slider_responseMarshaller;

        ///  Current slider value returned when saved or canceled
        unsigned int sliderPosition;  //!<  (1,26)
    };
}

#endif
