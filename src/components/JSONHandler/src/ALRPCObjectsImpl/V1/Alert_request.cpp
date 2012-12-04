#include "../include/JSONHandler/ALRPCObjects/V1/Alert_request.h"
#include "Alert_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Marshaller.h"
#include "TTSChunkMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV1;
Alert_request& Alert_request::operator =(const Alert_request& c)
{
  alertText1= c.alertText1 ? new std::string(c.alertText1[0]) : 0;
  alertText2= c.alertText2 ? new std::string(c.alertText2[0]) : 0;
  ttsChunks= c.ttsChunks ? new std::vector<TTSChunk>(c.ttsChunks[0]) : 0;
  duration= c.duration ? new unsigned int(c.duration[0]) : 0;
  playTone= c.playTone ? new bool(c.playTone[0]) : 0;

  return *this;}


Alert_request::~Alert_request(void)
{
  if(alertText1)
    delete alertText1;
  if(alertText2)
    delete alertText2;
  if(ttsChunks)
    delete ttsChunks;
  if(duration)
    delete duration;
  if(playTone)
    delete playTone;
}


Alert_request::Alert_request(const Alert_request& c)
{
  *this=c;
}


bool Alert_request::checkIntegrity(void)
{
  return Alert_requestMarshaller::checkIntegrity(*this);
}


Alert_request::Alert_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_ALERT_REQUEST),
      alertText1(0),
    alertText2(0),
    ttsChunks(0),
    duration(0),
    playTone(0)
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




const std::string* Alert_request::get_alertText1(void) const 
{
  return alertText1;
}

const std::string* Alert_request::get_alertText2(void) const 
{
  return alertText2;
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

