#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/Alert.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


Alert& Alert::operator =(const Alert& c)
{
  if(AlertText1)  delete AlertText1;
  AlertText1= c.AlertText1 ? new std::string(c.AlertText1[0]) : 0;
  if(AlertText2)  delete AlertText2;
  AlertText2= c.AlertText2 ? new std::string(c.AlertText2[0]) : 0;
  if(AlertText3)  delete AlertText3;
  AlertText3= c.AlertText3 ? new std::string(c.AlertText3[0]) : 0;
  if(ttsChunks)  delete ttsChunks;
  ttsChunks= c.ttsChunks ? new std::vector<NsSmartDeviceLinkRPCV2::TTSChunk>(c.ttsChunks[0]) : 0;
  if(duration)  delete duration;
  duration= c.duration ? new unsigned int(c.duration[0]) : 0;
  if(playTone)  delete playTone;
  playTone= c.playTone ? new bool(c.playTone[0]) : 0;
  if(softButtons)  delete softButtons;
  softButtons= c.softButtons ? new std::vector<NsSmartDeviceLinkRPCV2::SoftButton>(c.softButtons[0]) : 0;
  appId=c.appId;
  return *this;
}


Alert::~Alert(void)
{
  if(AlertText1)  delete AlertText1;
  if(AlertText2)  delete AlertText2;
  if(AlertText3)  delete AlertText3;
  if(ttsChunks)  delete ttsChunks;
  if(duration)  delete duration;
  if(playTone)  delete playTone;
  if(softButtons)  delete softButtons;
}


Alert::Alert(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ALERT),
  AlertText1(0),
  AlertText2(0),
  AlertText3(0),
  ttsChunks(0),
  duration(0),
  playTone(0),
  softButtons(0)
{
}


Alert::Alert(const Alert& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ALERT,c.getId())
{
  *this=c;
}


const std::string* Alert::get_AlertText1(void)
{
  return AlertText1;
}

bool Alert::set_AlertText1(const std::string& AlertText1_)
{
  if(AlertText1)  delete AlertText1;
  AlertText1=new std::string(AlertText1_);
  return true;
}

void Alert::reset_AlertText1(void)
{
  if(AlertText1)  delete AlertText1;
  AlertText1=0;
}

const std::string* Alert::get_AlertText2(void)
{
  return AlertText2;
}

bool Alert::set_AlertText2(const std::string& AlertText2_)
{
  if(AlertText2)  delete AlertText2;
  AlertText2=new std::string(AlertText2_);
  return true;
}

void Alert::reset_AlertText2(void)
{
  if(AlertText2)  delete AlertText2;
  AlertText2=0;
}

const std::string* Alert::get_AlertText3(void)
{
  return AlertText3;
}

bool Alert::set_AlertText3(const std::string& AlertText3_)
{
  if(AlertText3)  delete AlertText3;
  AlertText3=new std::string(AlertText3_);
  return true;
}

void Alert::reset_AlertText3(void)
{
  if(AlertText3)  delete AlertText3;
  AlertText3=0;
}

const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>* Alert::get_ttsChunks(void)
{
  return ttsChunks;
}

bool Alert::set_ttsChunks(const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& ttsChunks_)
{
  if(ttsChunks)  delete ttsChunks;
  ttsChunks=new std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>(ttsChunks_);
  return true;
}

void Alert::reset_ttsChunks(void)
{
  if(ttsChunks)  delete ttsChunks;
  ttsChunks=0;
}

const unsigned int* Alert::get_duration(void)
{
  return duration;
}

bool Alert::set_duration(const unsigned int& duration_)
{
  if(duration)  delete duration;
  duration=new unsigned int(duration_);
  return true;
}

void Alert::reset_duration(void)
{
  if(duration)  delete duration;
  duration=0;
}

const bool* Alert::get_playTone(void)
{
  return playTone;
}

bool Alert::set_playTone(const bool& playTone_)
{
  if(playTone)  delete playTone;
  playTone=new bool(playTone_);
  return true;
}

void Alert::reset_playTone(void)
{
  if(playTone)  delete playTone;
  playTone=0;
}

const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>* Alert::get_softButtons(void)
{
  return softButtons;
}

bool Alert::set_softButtons(const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>& softButtons_)
{
  if(softButtons)  delete softButtons;
  softButtons=new std::vector< NsSmartDeviceLinkRPCV2::SoftButton>(softButtons_);
  return true;
}

void Alert::reset_softButtons(void)
{
  if(softButtons)  delete softButtons;
  softButtons=0;
}

int Alert::get_appId(void)
{
  return appId;
}

bool Alert::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool Alert::checkIntegrity(void)
{
  return AlertMarshaller::checkIntegrity(*this);
}
