#include "../include/JSONHandler/ALRPCObjects/V2/Slider_response.h"
#include "Slider_responseMarshaller.h"

namespace
{
    const int PROTOCOL_VERSION = 2;
}

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    robok0der
*/

using namespace NsAppLinkRPCV2;
Slider_response& Slider_response::operator =(const Slider_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    sliderPosition = c.sliderPosition;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

Slider_response::~Slider_response(void)
{}

Slider_response::Slider_response(const Slider_response& c)
{
    *this = c;
}

bool Slider_response::checkIntegrity(void)
{
    return Slider_responseMarshaller::checkIntegrity(*this);
}

Slider_response::Slider_response(void) : NsAppLinkRPC::ALRPCResponse(PROTOCOL_VERSION)
{}

bool Slider_response::set_sliderPosition(unsigned int sliderPosition_)
{
    if (sliderPosition_ > 26)
    {
        return false;
    }
    if (sliderPosition_ < 1)
    {
        return false;
    }
    sliderPosition = sliderPosition_;
    return true;
}

unsigned int Slider_response::get_sliderPosition(void) const
{
    return sliderPosition;
}
