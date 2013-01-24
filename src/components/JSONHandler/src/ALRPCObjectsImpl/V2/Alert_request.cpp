#include "../include/JSONHandler/ALRPCObjects/V2/Alert_request.h"
#include "Alert_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "SoftButtonMarshaller.h"
#include "TTSChunkMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;
Alert_request& Alert_request::operator =(const Alert_request& c)
{
  alertText1= c.alertText1 ? new std::string(c.alertText1[0]) : 0;
  alertText2= c.alertText2 ? new std::string(c.alertText2[0]) : 0;
  alertText3= c.alertText3 ? new std::string(c.alertText3[0]) : 0;
  ttsChunks= c.ttsChunks ? new std::vector<TTSChunk>(c.ttsChunks[0]) : 0;
  duration= c.duration ? new unsigned int(c.duration[0]) : 0;
  playTone= c.playTone ? new bool(c.playTone[0]) : 0;
  softButtons= c.softButtons ? new std::vector<SoftButton>(c.softButtons[0]) : 0;

  return *this;
}


Alert_request::~Alert_request(void)
{
  if(alertText1)
    delete alertText1;
  if(alertText2)
    delete alertText2;
  if(alertText3)
    delete alertText3;
  if(ttsChunks)
    delete ttsChunks;
  if(duration)
    delete duration;
  if(playTone)
    delete playTone;
  if(softButtons)
    delete softButtons;
}


Alert_request::Alert_request(const Alert_request& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool Alert_request::checkIntegrity(void)
{
  return Alert_requestMarshaller::checkIntegrity(*this);
}


Alert_request::Alert_request(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION),
      alertText1(0),
    alertText2(0),
    alertText3(0),
    ttsChunks(0),
    duration(0),
    playTone(0),
    softButtons(0)
{
}



bool Alert_request::set_alertText1(const std::string& alertText1_)
{
  if(alertText1_.length()>500)  return false;
  delete alertText1;
  alertText1=0;

  alertText1=new std::string(alertText1_);
  return true;
}

void Alert_request::reset_alertText1(void)
{
  if(alertText1)
    delete alertText1;
  alertText1=0;
}

bool Alert_request::set_alertText2(const std::string& alertText2_)
{
  if(alertText2_.length()>500)  return false;
  delete alertText2;
  alertText2=0;

  alertText2=new std::string(alertText2_);
  return true;
}

void Alert_request::reset_alertText2(void)
{
  if(alertText2)
    delete alertText2;
  alertText2=0;
}

bool Alert_request::set_alertText3(const std::string& alertText3_)
{
  if(alertText3_.length()>500)  return false;
  delete alertText3;
  alertText3=0;

  alertText3=new std::string(alertText3_);
  return true;
}

void Alert_request::reset_alertText3(void)
{
  if(alertText3)
    delete alertText3;
  alertText3=0;
}

bool Alert_request::set_ttsChunks(const std::vector<TTSChunk>& ttsChunks_)
{
  unsigned int i=ttsChunks_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TTSChunkMarshaller::checkIntegrityConst(ttsChunks_[i]))   return false;
  }
  delete ttsChunks;
  ttsChunks=0;

  ttsChunks=new std::vector<TTSChunk>(ttsChunks_);
  return true;
}

void Alert_request::reset_ttsChunks(void)
{
  if(ttsChunks)
    delete ttsChunks;
  ttsChunks=0;
}

bool Alert_request::set_duration(unsigned int duration_)
{
  if(duration_>10000)  return false;
  if(duration_<3000)  return false;
  delete duration;
  duration=0;

  duration=new unsigned int(duration_);
  return true;
}

void Alert_request::reset_duration(void)
{
  if(duration)
    delete duration;
  duration=0;
}

bool Alert_request::set_playTone(bool playTone_)
{
  delete playTone;
  playTone=0;

  playTone=new bool(playTone_);
  return true;
}

void Alert_request::reset_playTone(void)
{
  if(playTone)
    delete playTone;
  playTone=0;
}

bool Alert_request::set_softButtons(const std::vector<SoftButton>& softButtons_)
{
  unsigned int i=softButtons_.size();
  if(i>4 || i<0)  return false;
  while(i--)
  {
    if(!SoftButtonMarshaller::checkIntegrityConst(softButtons_[i]))   return false;
  }
  delete softButtons;
  softButtons=0;

  softButtons=new std::vector<SoftButton>(softButtons_);
  return true;
}

void Alert_request::reset_softButtons(void)
{
  if(softButtons)
    delete softButtons;
  softButtons=0;
}




const std::string* Alert_request::get_alertText1(void) const 
{
  return alertText1;
}

const std::string* Alert_request::get_alertText2(void) const 
{
  return alertText2;
}

const std::string* Alert_request::get_alertText3(void) const 
{
  return alertText3;
}

const std::vector<TTSChunk>* Alert_request::get_ttsChunks(void) const 
{
  return ttsChunks;
}

const unsigned int* Alert_request::get_duration(void) const 
{
  return duration;
}

const bool* Alert_request::get_playTone(void) const 
{
  return playTone;
}

const std::vector<SoftButton>* Alert_request::get_softButtons(void) const 
{
  return softButtons;
}

