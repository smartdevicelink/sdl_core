#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteSubMenu.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DeleteSubMenuMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Nov 20 13:32:23 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool DeleteSubMenuMarshaller::checkIntegrity(DeleteSubMenu& s)
{
  return checkIntegrityConst(s);
}


bool DeleteSubMenuMarshaller::fromString(const std::string& s,DeleteSubMenu& e)
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


const std::string DeleteSubMenuMarshaller::toString(const DeleteSubMenu& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool DeleteSubMenuMarshaller::checkIntegrityConst(const DeleteSubMenu& s)
{
  if(s.menuId>2000000000)  return false;

  return true;
}


Json::Value DeleteSubMenuMarshaller::toJSON(const DeleteSubMenu& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.DeleteSubMenu");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["menuId"]=Json::Value(e.menuId);;
  return json;
}


bool DeleteSubMenuMarshaller::fromJSON(const Json::Value& json,DeleteSubMenu& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.DeleteSubMenu"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("menuId") || !js["menuId"].isInt())  return false;
    c.menuId=js["menuId"].asInt();
    if(c.menuId>2000000000)  return false;

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
