#include "OnDriverDistractionMarshaller.h"
#include "JSONHandler/RPC2Objects/UI/Marshaller.h"

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::UI;

OnDriverDistraction::OnDriverDistraction()
:RPC2Notification(Marshaller::METHOD_ONDRIVERDISTRACTION)
{}

OnDriverDistraction::~OnDriverDistraction()
{}

OnDriverDistraction& OnDriverDistraction::operator =(const OnDriverDistraction& c)
{
  state=c.state;
  return *this;
}

OnDriverDistraction::OnDriverDistraction(const OnDriverDistraction& c) : RPC2Notification(Marshaller::METHOD_ONDRIVERDISTRACTION)
{
  *this=c;
}

const AppLinkRPC::DriverDistractionState& OnDriverDistraction::get_state(void)
{
  return state;
}

bool OnDriverDistraction::set_state(const AppLinkRPC::DriverDistractionState& state_)
{
  state=state_;
  return true;
}

bool OnDriverDistraction::checkIntegrity(void)
{
  return OnDriverDistractionMarshaller::checkIntegrity(*this);
}
