#include "../include/JSONHandler/ALRPCObjects/V2/CreateInteractionChoiceSet_request.h"
#include "ChoiceMarshaller.h"

#include "CreateInteractionChoiceSet_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


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

  json["interactionChoiceSetID"]=Json::Value(e.interactionChoiceSetID);

  json["choiceSet"]=Json::Value(Json::arrayValue);
  json["choiceSet"].resize(e.choiceSet.size());
  for(unsigned int i=0;i<e.choiceSet.size();i++)
    json["choiceSet"][i]=ChoiceMarshaller::toJSON(e.choiceSet[i]);

  return json;
}


bool CreateInteractionChoiceSet_requestMarshaller::fromJSON(const Json::Value& json,CreateInteractionChoiceSet_request& c)
{
  try
  {
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

