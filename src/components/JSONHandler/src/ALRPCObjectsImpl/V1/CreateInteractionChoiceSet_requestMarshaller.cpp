#include "../include/JSONHandler/ALRPCObjects/V1/CreateInteractionChoiceSet_request.h"
#include "ChoiceMarshaller.h"

#include "CreateInteractionChoiceSet_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Dec 14 08:11:37 2012
  source stamp	Fri Dec 14 08:11:34 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool CreateInteractionChoiceSet_requestMarshaller::checkIntegrity(CreateInteractionChoiceSet_request& s)
{
  return checkIntegrityConst(s);
}


bool CreateInteractionChoiceSet_requestMarshaller::fromString(const std::string& s,CreateInteractionChoiceSet_request& e)
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


const std::string CreateInteractionChoiceSet_requestMarshaller::toString(const CreateInteractionChoiceSet_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool CreateInteractionChoiceSet_requestMarshaller::checkIntegrityConst(const CreateInteractionChoiceSet_request& s)
{
  if(s.interactionChoiceSetID>2000000000)  return false;
  {
    unsigned int i=s.choiceSet.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!ChoiceMarshaller::checkIntegrityConst(s.choiceSet[i]))   return false;
    }
  }
  return true;
}

Json::Value CreateInteractionChoiceSet_requestMarshaller::toJSON(const CreateInteractionChoiceSet_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("CreateInteractionChoiceSet");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["interactionChoiceSetID"]=Json::Value(e.interactionChoiceSetID);

  j["choiceSet"]=Json::Value(Json::arrayValue);
  j["choiceSet"].resize(e.choiceSet.size());
  for(unsigned int i=0;i<e.choiceSet.size();i++)
    j["choiceSet"][i]=ChoiceMarshaller::toJSON(e.choiceSet[i]);

  json["request"]["parameters"]=j;
  return json;
}


bool CreateInteractionChoiceSet_requestMarshaller::fromJSON(const Json::Value& js,CreateInteractionChoiceSet_request& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("CreateInteractionChoiceSet"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("interactionChoiceSetID"))  return false;
    {
      const Json::Value& j=json["interactionChoiceSetID"];
      if(!j.isInt())  return false;
      c.interactionChoiceSetID=j.asInt();
    }
    if(!json.isMember("choiceSet"))  return false;
    {
      const Json::Value& j=json["choiceSet"];
      if(!j.isArray())  return false;
      c.choiceSet.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          Choice t;
          if(!ChoiceMarshaller::fromJSON(j[i],t))
            return false;
          c.choiceSet[i]=t;
        }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

