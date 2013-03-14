#include "../include/JSONHandler/SDLRPCObjects/V1/DeleteInteractionChoiceSet_request.h"


#include "DeleteInteractionChoiceSet_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

using namespace NsSmartDeviceLinkRPC;


bool DeleteInteractionChoiceSet_requestMarshaller::checkIntegrity(DeleteInteractionChoiceSet_request& s)
{
  return checkIntegrityConst(s);
}


bool DeleteInteractionChoiceSet_requestMarshaller::fromString(const std::string& s,DeleteInteractionChoiceSet_request& e)
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


const std::string DeleteInteractionChoiceSet_requestMarshaller::toString(const DeleteInteractionChoiceSet_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool DeleteInteractionChoiceSet_requestMarshaller::checkIntegrityConst(const DeleteInteractionChoiceSet_request& s)
{
  if(s.interactionChoiceSetID>2000000000)  return false;
  return true;
}

Json::Value DeleteInteractionChoiceSet_requestMarshaller::toJSON(const DeleteInteractionChoiceSet_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("DeleteInteractionChoiceSet");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["interactionChoiceSetID"]=Json::Value(e.interactionChoiceSetID);

  json["request"]["parameters"]=j;
  return json;
}


bool DeleteInteractionChoiceSet_requestMarshaller::fromJSON(const Json::Value& js,DeleteInteractionChoiceSet_request& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("DeleteInteractionChoiceSet"))  return false;
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

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

