#include "../include/JSONHandler/ALRPCObjects/V2/PerformInteraction_request.h"
#include "InteractionModeMarshaller.h"
#include "TTSChunkMarshaller.h"
#include "VrHelpItemMarshaller.h"

#include "PerformInteraction_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


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

  json["initialText"]=Json::Value(e.initialText);

  json["initialPrompt"]=Json::Value(Json::arrayValue);
  json["initialPrompt"].resize(e.initialPrompt.size());
  for(unsigned int i=0;i<e.initialPrompt.size();i++)
    json["initialPrompt"][i]=TTSChunkMarshaller::toJSON(e.initialPrompt[i]);

  json["interactionMode"]=InteractionModeMarshaller::toJSON(e.interactionMode);

  json["interactionChoiceSetIDList"]=Json::Value(Json::arrayValue);
  json["interactionChoiceSetIDList"].resize(e.interactionChoiceSetIDList.size());
  for(unsigned int i=0;i<e.interactionChoiceSetIDList.size();i++)
    json["interactionChoiceSetIDList"][i]=Json::Value(e.interactionChoiceSetIDList[i]);

  if(e.helpPrompt)
  {
    unsigned int sz=e.helpPrompt->size();
    json["helpPrompt"]=Json::Value(Json::arrayValue);
    json["helpPrompt"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["helpPrompt"][i]=TTSChunkMarshaller::toJSON(e.helpPrompt[0][i]);
  }

  if(e.timeoutPrompt)
  {
    unsigned int sz=e.timeoutPrompt->size();
    json["timeoutPrompt"]=Json::Value(Json::arrayValue);
    json["timeoutPrompt"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["timeoutPrompt"][i]=TTSChunkMarshaller::toJSON(e.timeoutPrompt[0][i]);
  }

  if(e.timeout)
    json["timeout"]=Json::Value(*e.timeout);

  if(e.vrHelp)
  {
    unsigned int sz=e.vrHelp->size();
    json["vrHelp"]=Json::Value(Json::arrayValue);
    json["vrHelp"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["vrHelp"][i]=VrHelpItemMarshaller::toJSON(e.vrHelp[0][i]);
  }

  return json;
}


bool PerformInteraction_requestMarshaller::fromJSON(const Json::Value& json,PerformInteraction_request& c)
{
  if(c.helpPrompt)  delete c.helpPrompt;
  c.helpPrompt=0;

  if(c.timeoutPrompt)  delete c.timeoutPrompt;
  c.timeoutPrompt=0;

  if(c.timeout)  delete c.timeout;
  c.timeout=0;

  if(c.vrHelp)  delete c.vrHelp;
  c.vrHelp=0;

  try
  {
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

