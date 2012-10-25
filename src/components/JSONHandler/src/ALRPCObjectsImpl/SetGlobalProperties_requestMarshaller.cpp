#include "../../include/JSONHandler/ALRPCObjects/SetGlobalProperties_request.h"
#include "TTSChunkMarshaller.h"

#include "SetGlobalProperties_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


bool SetGlobalProperties_requestMarshaller::checkIntegrity(SetGlobalProperties_request& s)
{
  return checkIntegrityConst(s);
}


bool SetGlobalProperties_requestMarshaller::fromString(const std::string& s,SetGlobalProperties_request& e)
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


const std::string SetGlobalProperties_requestMarshaller::toString(const SetGlobalProperties_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SetGlobalProperties_requestMarshaller::checkIntegrityConst(const SetGlobalProperties_request& s)
{
  if(s.helpPrompt)
  {
    unsigned int i=s.helpPrompt[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TTSChunkMarshaller::checkIntegrityConst(s.helpPrompt[0][i]))   return false;
    }
  }
  if(s.timeoutPrompt)
  {
    unsigned int i=s.timeoutPrompt[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TTSChunkMarshaller::checkIntegrityConst(s.timeoutPrompt[0][i]))   return false;
    }
  }
  return true;
}

Json::Value SetGlobalProperties_requestMarshaller::toJSON(const SetGlobalProperties_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("SetGlobalProperties");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  if(e.helpPrompt)
  {
    unsigned int sz=e.helpPrompt->size();
    j["helpPrompt"]=Json::Value(Json::arrayValue);
    j["helpPrompt"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["helpPrompt"][i]=TTSChunkMarshaller::toJSON(e.helpPrompt[0][i]);
  }

  if(e.timeoutPrompt)
  {
    unsigned int sz=e.timeoutPrompt->size();
    j["timeoutPrompt"]=Json::Value(Json::arrayValue);
    j["timeoutPrompt"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["timeoutPrompt"][i]=TTSChunkMarshaller::toJSON(e.timeoutPrompt[0][i]);
  }

  json["request"]["parameters"]=j;
  return json;
}


bool SetGlobalProperties_requestMarshaller::fromJSON(const Json::Value& js,SetGlobalProperties_request& c)
{
  if(c.helpPrompt)  delete c.helpPrompt;
  c.helpPrompt=0;

  if(c.timeoutPrompt)  delete c.timeoutPrompt;
  c.timeoutPrompt=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("SetGlobalProperties"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(json.isMember("helpPrompt"))
    {
      const Json::Value& j=json["helpPrompt"];
      if(!j.isArray())  return false;
      c.helpPrompt=new std::vector<TTSChunk>();
      c.helpPrompt->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        TTSChunk t;
        if(!TTSChunkMarshaller::fromJSON(j[i],t))
          return false;
        c.helpPrompt[0][i]=t;
      }

    }
    if(json.isMember("timeoutPrompt"))
    {
      const Json::Value& j=json["timeoutPrompt"];
      if(!j.isArray())  return false;
      c.timeoutPrompt=new std::vector<TTSChunk>();
      c.timeoutPrompt->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        TTSChunk t;
        if(!TTSChunkMarshaller::fromJSON(j[i],t))
          return false;
        c.timeoutPrompt[0][i]=t;
      }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

