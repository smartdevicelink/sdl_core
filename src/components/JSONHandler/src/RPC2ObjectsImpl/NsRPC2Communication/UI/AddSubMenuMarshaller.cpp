#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AddSubMenu.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AddSubMenuMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool AddSubMenuMarshaller::checkIntegrity(AddSubMenu& s)
{
  return checkIntegrityConst(s);
}


bool AddSubMenuMarshaller::fromString(const std::string& s,AddSubMenu& e)
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


const std::string AddSubMenuMarshaller::toString(const AddSubMenu& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool AddSubMenuMarshaller::checkIntegrityConst(const AddSubMenu& s)
{
  if(s.menuId>2000000000)  return false;

  if(s.position && (s.position[0]>1000))  return false;

  if(s.menuName.length()>500)  return false;

  return true;
}


Json::Value AddSubMenuMarshaller::toJSON(const AddSubMenu& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.AddSubMenu");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["menuId"]=Json::Value(e.menuId);;
  if(e.position)
    json["params"]["position"]=Json::Value(e.position[0]);;
  json["params"]["menuName"]=Json::Value(e.menuName);;
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool AddSubMenuMarshaller::fromJSON(const Json::Value& json,AddSubMenu& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.AddSubMenu"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("menuId") || !js["menuId"].isInt())  return false;
    c.menuId=js["menuId"].asInt();
    if(c.menuId>2000000000)  return false;

    if(c.position)  delete c.position;
    c.position=0;
    if(js.isMember("position"))
    {
      if(!js["position"].isInt())  return false;
      c.position=new unsigned int();
      c.position[0]=js["position"].asInt();
      if(c.position[0]>1000)  return false;

    }

    if(!js.isMember("menuName") || !js["menuName"].isString())  return false;
    c.menuName=js["menuName"].asString();
    if(c.menuName.length()>500)  return false;

    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
