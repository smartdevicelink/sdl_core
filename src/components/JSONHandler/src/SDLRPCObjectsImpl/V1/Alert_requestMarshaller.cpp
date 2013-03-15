//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V1/Alert_request.h"
#include "TTSChunkMarshaller.h"

#include "Alert_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPC;


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
  return true;
}

Json::Value Alert_requestMarshaller::toJSON(const Alert_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("Alert");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  if(e.alertText1)
    j["alertText1"]=Json::Value(*e.alertText1);

  if(e.alertText2)
    j["alertText2"]=Json::Value(*e.alertText2);

  if(e.ttsChunks)
  {
    unsigned int sz=e.ttsChunks->size();
    j["ttsChunks"]=Json::Value(Json::arrayValue);
    j["ttsChunks"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["ttsChunks"][i]=TTSChunkMarshaller::toJSON(e.ttsChunks[0][i]);
  }

  if(e.duration)
    j["duration"]=Json::Value(*e.duration);

  if(e.playTone)
    j["playTone"]=Json::Value(*e.playTone);

  json["request"]["parameters"]=j;
  return json;
}


bool Alert_requestMarshaller::fromJSON(const Json::Value& js,Alert_request& c)
{
  if(c.alertText1)  delete c.alertText1;
  c.alertText1=0;

  if(c.alertText2)  delete c.alertText2;
  c.alertText2=0;

  if(c.ttsChunks)  delete c.ttsChunks;
  c.ttsChunks=0;

  if(c.duration)  delete c.duration;
  c.duration=0;

  if(c.playTone)  delete c.playTone;
  c.playTone=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("Alert"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
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

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

