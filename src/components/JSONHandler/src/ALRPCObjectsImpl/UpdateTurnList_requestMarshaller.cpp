#include "../../include/JSONHandler/ALRPCObjects/UpdateTurnList_request.h"
#include "SoftButtonMarshaller.h"
#include "TurnMarshaller.h"

#include "UpdateTurnList_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


bool UpdateTurnList_requestMarshaller::checkIntegrity(UpdateTurnList_request& s)
{
  return checkIntegrityConst(s);
}


bool UpdateTurnList_requestMarshaller::fromString(const std::string& s,UpdateTurnList_request& e)
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


const std::string UpdateTurnList_requestMarshaller::toString(const UpdateTurnList_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool UpdateTurnList_requestMarshaller::checkIntegrityConst(const UpdateTurnList_request& s)
{
  {
    unsigned int i=s.turnList.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TurnMarshaller::checkIntegrityConst(s.turnList[i]))   return false;
    }
  }
  {
    unsigned int i=s.softButtons.size();
    if(i>1 || i<0)  return false;
    while(i--)
    {
    if(!SoftButtonMarshaller::checkIntegrityConst(s.softButtons[i]))   return false;
    }
  }
  return true;
}

Json::Value UpdateTurnList_requestMarshaller::toJSON(const UpdateTurnList_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("UpdateTurnList");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["turnList"]=Json::Value(Json::arrayValue);
  j["turnList"].resize(e.turnList.size());
  for(unsigned int i=0;i<e.turnList.size();i++)
    j["turnList"][i]=TurnMarshaller::toJSON(e.turnList[i]);

  j["softButtons"]=Json::Value(Json::arrayValue);
  j["softButtons"].resize(e.softButtons.size());
  for(unsigned int i=0;i<e.softButtons.size();i++)
    j["softButtons"][i]=SoftButtonMarshaller::toJSON(e.softButtons[i]);

  json["request"]["parameters"]=j;
  return json;
}


bool UpdateTurnList_requestMarshaller::fromJSON(const Json::Value& js,UpdateTurnList_request& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("UpdateTurnList"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("turnList"))  return false;
    {
      const Json::Value& j=json["turnList"];
      if(!j.isArray())  return false;
      c.turnList.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          Turn t;
          if(!TurnMarshaller::fromJSON(j[i],t))
            return false;
          c.turnList[i]=t;
        }

    }
    if(!json.isMember("softButtons"))  return false;
    {
      const Json::Value& j=json["softButtons"];
      if(!j.isArray())  return false;
      c.softButtons.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          SoftButton t;
          if(!SoftButtonMarshaller::fromJSON(j[i],t))
            return false;
          c.softButtons[i]=t;
        }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

