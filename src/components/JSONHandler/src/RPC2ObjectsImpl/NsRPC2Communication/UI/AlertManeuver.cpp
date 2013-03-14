#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AlertManeuver.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


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
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ALERTMANEUVER)
{
}


AlertManeuver::AlertManeuver(const AlertManeuver& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ALERTMANEUVER,c.getId())
{
  *this=c;
}


const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& AlertManeuver::get_ttsChunks(void)
{
  return ttsChunks;
}

bool AlertManeuver::set_ttsChunks(const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& ttsChunks_)
{
  ttsChunks=ttsChunks_;
  return true;
}

const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>& AlertManeuver::get_softButtons(void)
{
  return softButtons;
}

bool AlertManeuver::set_softButtons(const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>& softButtons_)
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
