#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Navigation/AlertManeuver.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::Navigation
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Navigation;


AlertManeuver& AlertManeuver::operator =(const AlertManeuver& c)
{
  ttsChunks=c.ttsChunks;
  softButtons=c.softButtons;
  appId=c.appId;
  return *this;
}


AlertManeuver::~AlertManeuver(void)
{
}


AlertManeuver::AlertManeuver(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_NAVIGATION__ALERTMANEUVER)
{
}


AlertManeuver::AlertManeuver(const AlertManeuver& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_NAVIGATION__ALERTMANEUVER,c.getId())
{
  *this=c;
}


const std::vector< NsAppLinkRPC::TTSChunk>& AlertManeuver::get_ttsChunks(void)
{
  return ttsChunks;
}

bool AlertManeuver::set_ttsChunks(const std::vector< NsAppLinkRPC::TTSChunk>& ttsChunks_)
{
  ttsChunks=ttsChunks_;
  return true;
}

const std::vector< NsAppLinkRPC::SoftButton>& AlertManeuver::get_softButtons(void)
{
  return softButtons;
}

bool AlertManeuver::set_softButtons(const std::vector< NsAppLinkRPC::SoftButton>& softButtons_)
{
  softButtons=softButtons_;
  return true;
}

int AlertManeuver::get_appId(void)
{
  return appId;
}

bool AlertManeuver::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool AlertManeuver::checkIntegrity(void)
{
  return AlertManeuverMarshaller::checkIntegrity(*this);
}
