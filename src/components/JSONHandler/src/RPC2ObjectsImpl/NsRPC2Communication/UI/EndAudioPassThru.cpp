//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/EndAudioPassThru.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


EndAudioPassThru& EndAudioPassThru::operator =(const EndAudioPassThru& c)
{
  appId=c.appId;
  return *this;
}


EndAudioPassThru::~EndAudioPassThru(void)
{
}


EndAudioPassThru::EndAudioPassThru(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ENDAUDIOPASSTHRU)
{
}


EndAudioPassThru::EndAudioPassThru(const EndAudioPassThru& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ENDAUDIOPASSTHRU,c.getId())
{
  *this=c;
}


int EndAudioPassThru::get_appId(void)
{
  return appId;
}

bool EndAudioPassThru::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool EndAudioPassThru::checkIntegrity(void)
{
  return EndAudioPassThruMarshaller::checkIntegrity(*this);
}
