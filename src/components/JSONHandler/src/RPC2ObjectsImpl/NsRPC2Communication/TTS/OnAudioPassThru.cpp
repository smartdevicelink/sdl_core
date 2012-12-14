#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/OnAudioPassThru.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::TTS;


OnAudioPassThru& OnAudioPassThru::operator =(const OnAudioPassThru& c)
{
  appId=c.appId;
  return *this;
}


OnAudioPassThru::~OnAudioPassThru(void)
{
}


OnAudioPassThru::OnAudioPassThru(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__ONAUDIOPASSTHRU)
{
}


OnAudioPassThru::OnAudioPassThru(const OnAudioPassThru& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__ONAUDIOPASSTHRU)
{
  *this=c;
}


int OnAudioPassThru::get_appId(void)
{
  return appId;
}

bool OnAudioPassThru::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool OnAudioPassThru::checkIntegrity(void)
{
  return OnAudioPassThruMarshaller::checkIntegrity(*this);
}
