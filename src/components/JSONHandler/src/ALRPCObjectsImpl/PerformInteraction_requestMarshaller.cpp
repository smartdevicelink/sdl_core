#include "../../include/JSONHandler/ALRPCObjects/PerformInteraction_request.h"
#include "InteractionModeMarshaller.h"
#include "TTSChunkMarshaller.h"
#include "VrHelpItemMarshaller.h"

#include "PerformInteraction_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


bool PerformInteraction_requestMarshaller::checkIntegrity(PerformInteraction_request& s)
{
  return checkIntegrityConst(s);
}


bool PerformInteraction_requestMarshaller::fromString(const std::string& s,PerformInteraction_request& e)
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


const std::string PerformInteraction_requestMarshaller::toString(const PerformInteraction_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool PerformInteraction_requestMarshaller::checkIntegrityConst(const PerformInteraction_request& s)
{
  if(s.initialText.length()>500)  return false;
  {
    unsigned int i=s.initialPrompt.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TTSChunkMarshaller::checkIntegrityConst(s.initialPrompt[i]))   return false;
    }
  }
  if(!InteractionModeMarshaller::checkIntegrityConst(s.interactionMode))  return false;
  {
    unsigned int i=s.interactionChoiceSetIDList.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
      if(s.interactionChoiceSetIDList[i]>2000000000)  return false;
    }
  }
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
  if(s.timeout && *s.timeout>100000)  return false;
  if(s.timeout && *s.timeout<5000)  return false;
  if(s.vrHelpTitle && s.vrHelpTitle->length()>500)  return false;
  if(s.vrHelp)
  {
    unsigned int i=s.vrHelp[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!VrHelpItemMarshaller::checkIntegrityConst(s.vrHelp[0][i]))   return false;
    }
  }
  return true;
}

Json::Value PerformInteraction_requestMarshaller::toJSON(const PerformInteraction_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("PerformInteraction");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["initialText"]=Json::Value(e.initialText);

  j["initialPrompt"]=Json::Value(Json::arrayValue);
  j["initialPrompt"].resize(e.initialPrompt.size());
  for(unsigned int i=0;i<e.initialPrompt.size();i++)
    j["initialPrompt"][i]=TTSChunkMarshaller::toJSON(e.initialPrompt[i]);

  j["interactionMode"]=InteractionModeMarshaller::toJSON(e.interactionMode);

  j["interactionChoiceSetIDList"]=Json::Value(Json::arrayValue);
  j["interactionChoiceSetIDList"].resize(e.interactionChoiceSetIDList.size());
  for(unsigned int i=0;i<e.interactionChoiceSetIDList.size();i++)
    j["interactionChoiceSetIDList"][i]=Json::Value(e.interactionChoiceSetIDList[i]);

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

  if(e.timeout)
    j["timeout"]=Json::Value(*e.timeout);

  if(e.vrHelpTitle)
    j["vrHelpTitle"]=Json::Value(*e.vrHelpTitle);

  if(e.vrHelp)
  {
    unsigned int sz=e.vrHelp->size();
    j["vrHelp"]=Json::Value(Json::arrayValue);
    j["vrHelp"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["vrHelp"][i]=VrHelpItemMarshaller::toJSON(e.vrHelp[0][i]);
  }

  json["request"]["parameters"]=j;
  return json;
}


bool PerformInteraction_requestMarshaller::fromJSON(const Json::Value& js,PerformInteraction_request& c)
{
  if(c.helpPrompt)  delete c.helpPrompt;
  c.helpPrompt=0;

  if(c.timeoutPrompt)  delete c.timeoutPrompt;
  c.timeoutPrompt=0;

  if(c.timeout)  delete c.timeout;
  c.timeout=0;

  if(c.vrHelpTitle)  delete c.vrHelpTitle;
  c.vrHelpTitle=0;

  if(c.vrHelp)  delete c.vrHelp;
  c.vrHelp=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("PerformInteraction"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("initialText"))  return false;
    {
      const Json::Value& j=json["initialText"];
      if(!j.isString())  return false;
      c.initialText=j.asString();
    }
    if(!json.isMember("initialPrompt"))  return false;
    {
      const Json::Value& j=json["initialPrompt"];
      if(!j.isArray())  return false;
      c.initialPrompt.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          TTSChunk t;
          if(!TTSChunkMarshaller::fromJSON(j[i],t))
            return false;
          c.initialPrompt[i]=t;
        }

    }
    if(!json.isMember("interactionMode"))  return false;
    {
      const Json::Value& j=json["interactionMode"];
      if(!InteractionModeMarshaller::fromJSON(j,c.interactionMode))
        return false;
    }
    if(!json.isMember("interactionChoiceSetIDList"))  return false;
    {
      const Json::Value& j=json["interactionChoiceSetIDList"];
      if(!j.isArray())  return false;
      c.interactionChoiceSetIDList.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        if(!j[i].isInt())
          return false;
        else
          c.interactionChoiceSetIDList[i]=j[i].asInt();
    }
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
    if(json.isMember("timeout"))
    {
      const Json::Value& j=json["timeout"];
      if(!j.isInt())  return false;
      c.timeout=new unsigned int(j.asInt());
    }
    if(json.isMember("vrHelpTitle"))
    {
      const Json::Value& j=json["vrHelpTitle"];
      if(!j.isString())  return false;
      c.vrHelpTitle=new std::string(j.asString());
    }
    if(json.isMember("vrHelp"))
    {
      const Json::Value& j=json["vrHelp"];
      if(!j.isArray())  return false;
      c.vrHelp=new std::vector<VrHelpItem>();
      c.vrHelp->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        VrHelpItem t;
        if(!VrHelpItemMarshaller::fromJSON(j[i],t))
          return false;
        c.vrHelp[0][i]=t;
      }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

