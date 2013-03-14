#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteInteractionChoiceSet.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DeleteInteractionChoiceSetMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool DeleteInteractionChoiceSetMarshaller::checkIntegrity(DeleteInteractionChoiceSet& s)
{
  return checkIntegrityConst(s);
}


bool DeleteInteractionChoiceSetMarshaller::fromString(const std::string& s,DeleteInteractionChoiceSet& e)
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


const std::string DeleteInteractionChoiceSetMarshaller::toString(const DeleteInteractionChoiceSet& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool DeleteInteractionChoiceSetMarshaller::checkIntegrityConst(const DeleteInteractionChoiceSet& s)
{
  if(s.interactionChoiceSetID>2000000000)  return false;

  return true;
}


Json::Value DeleteInteractionChoiceSetMarshaller::toJSON(const DeleteInteractionChoiceSet& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.DeleteInteractionChoiceSet");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["interactionChoiceSetID"]=Json::Value(e.interactionChoiceSetID);;
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool DeleteInteractionChoiceSetMarshaller::fromJSON(const Json::Value& json,DeleteInteractionChoiceSet& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.DeleteInteractionChoiceSet"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("interactionChoiceSetID") || !js["interactionChoiceSetID"].isInt())  return false;
    c.interactionChoiceSetID=js["interactionChoiceSetID"].asInt();
    if(c.interactionChoiceSetID>2000000000)  return false;

    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
