#include "../include/JSONHandler/ALRPCObjects/V2/Alert_request.h"
#include "SoftButtonMarshaller.h"
#include "TTSChunkMarshaller.h"

#include "Alert_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool Alert_requestMarshaller::checkIntegrity(Alert_request& s)
{
  return checkIntegrityConst(s);
}


bool Alert_requestMarshaller::fromString(const std::string& s,Alert_request& e)
{
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(!fromJSON(json,e))  return false;
  }
  catch(...)
  {
    return false;
  }
  return true;
}


const std::string Alert_requestMarshaller::toString(const Alert_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool Alert_requestMarshaller::checkIntegrityConst(const Alert_request& s)
{
  if(s.alertText1 && s.alertText1->length()>500)  return false;
  if(s.alertText2 && s.alertText2->length()>500)  return false;
  if(s.alertText3 && s.alertText3->length()>500)  return false;
  if(s.ttsChunks)
  {
    unsigned int i=s.ttsChunks[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TTSChunkMarshaller::checkIntegrityConst(s.ttsChunks[0][i]))   return false;
    }
  }
  if(s.duration && *s.duration>10000)  return false;
  if(s.duration && *s.duration<3000)  return false;
  if(s.softButtons)
  {
    unsigned int i=s.softButtons[0].size();
    if(i>4 || i<0)  return false;
    while(i--)
    {
    if(!SoftButtonMarshaller::checkIntegrityConst(s.softButtons[0][i]))   return false;
    }
  }
  return true;
}

Json::Value Alert_requestMarshaller::toJSON(const Alert_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  if(e.alertText1)
    json["alertText1"]=Json::Value(*e.alertText1);

  if(e.alertText2)
    json["alertText2"]=Json::Value(*e.alertText2);

  if(e.alertText3)
    json["alertText3"]=Json::Value(*e.alertText3);

  if(e.ttsChunks)
  {
    unsigned int sz=e.ttsChunks->size();
    json["ttsChunks"]=Json::Value(Json::arrayValue);
    json["ttsChunks"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["ttsChunks"][i]=TTSChunkMarshaller::toJSON(e.ttsChunks[0][i]);
  }

  if(e.duration)
    json["duration"]=Json::Value(*e.duration);

  if(e.playTone)
    json["playTone"]=Json::Value(*e.playTone);

  if(e.softButtons)
  {
    unsigned int sz=e.softButtons->size();
    json["softButtons"]=Json::Value(Json::arrayValue);
    json["softButtons"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["softButtons"][i]=SoftButtonMarshaller::toJSON(e.softButtons[0][i]);
  }

  return json;
}


bool Alert_requestMarshaller::fromJSON(const Json::Value& json,Alert_request& c)
{
  if(c.alertText1)  delete c.alertText1;
  c.alertText1=0;

  if(c.alertText2)  delete c.alertText2;
  c.alertText2=0;

  if(c.alertText3)  delete c.alertText3;
  c.alertText3=0;

  if(c.ttsChunks)  delete c.ttsChunks;
  c.ttsChunks=0;

  if(c.duration)  delete c.duration;
  c.duration=0;

  if(c.playTone)  delete c.playTone;
  c.playTone=0;

  if(c.softButtons)  delete c.softButtons;
  c.softButtons=0;

  try
  {
    if(!json.isObject())  return false;

    if(json.isMember("alertText1"))
    {
      const Json::Value& j=json["alertText1"];
      if(!j.isString())  return false;
      c.alertText1=new std::string(j.asString());
    }
    if(json.isMember("alertText2"))
    {
      const Json::Value& j=json["alertText2"];
      if(!j.isString())  return false;
      c.alertText2=new std::string(j.asString());
    }
    if(json.isMember("alertText3"))
    {
      const Json::Value& j=json["alertText3"];
      if(!j.isString())  return false;
      c.alertText3=new std::string(j.asString());
    }
    if(json.isMember("ttsChunks"))
    {
      const Json::Value& j=json["ttsChunks"];
      if(!j.isArray())  return false;
      c.ttsChunks=new std::vector<TTSChunk>();
      c.ttsChunks->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        TTSChunk t;
        if(!TTSChunkMarshaller::fromJSON(j[i],t))
          return false;
        c.ttsChunks[0][i]=t;
      }

    }
    if(json.isMember("duration"))
    {
      const Json::Value& j=json["duration"];
      if(!j.isInt())  return false;
      c.duration=new unsigned int(j.asInt());
    }
    if(json.isMember("playTone"))
    {
      const Json::Value& j=json["playTone"];
      if(!j.isBool())  return false;
      c.playTone=new bool(j.asBool());
    }
    if(json.isMember("softButtons"))
    {
      const Json::Value& j=json["softButtons"];
      if(!j.isArray())  return false;
      c.softButtons=new std::vector<SoftButton>();
      c.softButtons->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        SoftButton t;
        if(!SoftButtonMarshaller::fromJSON(j[i],t))
          return false;
        c.softButtons[0][i]=t;
      }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

