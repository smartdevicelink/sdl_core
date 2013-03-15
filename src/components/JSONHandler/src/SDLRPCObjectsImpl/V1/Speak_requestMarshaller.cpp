//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V1/Speak_request.h"
#include "TTSChunkMarshaller.h"

#include "Speak_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPC;


bool Speak_requestMarshaller::checkIntegrity(Speak_request& s)
{
  return checkIntegrityConst(s);
}


bool Speak_requestMarshaller::fromString(const std::string& s,Speak_request& e)
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


const std::string Speak_requestMarshaller::toString(const Speak_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool Speak_requestMarshaller::checkIntegrityConst(const Speak_request& s)
{
  {
    unsigned int i=s.ttsChunks.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TTSChunkMarshaller::checkIntegrityConst(s.ttsChunks[i]))   return false;
    }
  }
  return true;
}

Json::Value Speak_requestMarshaller::toJSON(const Speak_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("Speak");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["ttsChunks"]=Json::Value(Json::arrayValue);
  j["ttsChunks"].resize(e.ttsChunks.size());
  for(unsigned int i=0;i<e.ttsChunks.size();i++)
    j["ttsChunks"][i]=TTSChunkMarshaller::toJSON(e.ttsChunks[i]);

  json["request"]["parameters"]=j;
  return json;
}


bool Speak_requestMarshaller::fromJSON(const Json::Value& js,Speak_request& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("Speak"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("ttsChunks"))  return false;
    {
      const Json::Value& j=json["ttsChunks"];
      if(!j.isArray())  return false;
      c.ttsChunks.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          TTSChunk t;
          if(!TTSChunkMarshaller::fromJSON(j[i],t))
            return false;
          c.ttsChunks[i]=t;
        }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

