#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteInteractionChoiceSetResponse.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "DeleteInteractionChoiceSetResponseMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 08:20:05 2012
  source stamp	Mon Nov 19 07:04:40 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool DeleteInteractionChoiceSetResponseMarshaller::checkIntegrity(DeleteInteractionChoiceSetResponse& s)
{
  return checkIntegrityConst(s);
}


bool DeleteInteractionChoiceSetResponseMarshaller::fromString(const std::string& s,DeleteInteractionChoiceSetResponse& e)
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


const std::string DeleteInteractionChoiceSetResponseMarshaller::toString(const DeleteInteractionChoiceSetResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool DeleteInteractionChoiceSetResponseMarshaller::checkIntegrityConst(const DeleteInteractionChoiceSetResponse& s)
{
  return true;
}


Json::Value DeleteInteractionChoiceSetResponseMarshaller::toJSON(const DeleteInteractionChoiceSetResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsAppLinkRPC::Result r(static_cast<NsAppLinkRPC::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsAppLinkRPC::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("UI.DeleteInteractionChoiceSetResponse");

  return json;
}


bool DeleteInteractionChoiceSetResponseMarshaller::fromJSON(const Json::Value& json,DeleteInteractionChoiceSetResponse& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("result")) return false;

    Json::Value js=json["result"];
    if(!js.isObject())  return false;

    NsAppLinkRPC::Result r;
    if(!js.isMember("resultCode") || !js["resultCode"].isString())  return false;
    if(!js.isMember("method") || !js["method"].isString())  return false;
    if(js["method"].asString().compare("UI.DeleteInteractionChoiceSetResponse")) return false;

    if(!NsAppLinkRPC::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
