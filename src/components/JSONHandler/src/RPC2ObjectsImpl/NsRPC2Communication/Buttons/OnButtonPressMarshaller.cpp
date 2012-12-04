#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/OnButtonPress.h"
#include "../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/ButtonPressModeMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/Buttons/OnButtonPressMarshaller.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Buttons;

bool OnButtonPressMarshaller::checkIntegrity(OnButtonPress& s)
{
  return checkIntegrityConst(s);
}


bool OnButtonPressMarshaller::fromString(const std::string& s,OnButtonPress& e)
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


const std::string OnButtonPressMarshaller::toString(const OnButtonPress& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnButtonPressMarshaller::checkIntegrityConst(const OnButtonPress& s)
{
  if(!NsAppLinkRPC::ButtonNameMarshaller::checkIntegrityConst(s.name))  return false;

  if(!NsAppLinkRPC::ButtonPressModeMarshaller::checkIntegrityConst(s.mode))  return false;

  if(s.customButtonName && (s.customButtonName[0].length()>500))  return false;

  return true;
}


Json::Value OnButtonPressMarshaller::toJSON(const OnButtonPress& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("Buttons.OnButtonPress");
  json["params"]=Json::Value(Json::objectValue);

  json["params"]["name"]=NsAppLinkRPC::ButtonNameMarshaller::toJSON(e.name);;
  json["params"]["mode"]=NsAppLinkRPC::ButtonPressModeMarshaller::toJSON(e.mode);;
  if(e.customButtonName)
    json["params"]["customButtonName"]=Json::Value(e.customButtonName[0]);;
  return json;
}


bool OnButtonPressMarshaller::fromJSON(const Json::Value& json,OnButtonPress& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("Buttons.OnButtonPress"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

    if(!js.isMember("name") || !NsAppLinkRPC::ButtonNameMarshaller::fromJSON(js["name"],c.name))  return false;

    if(!js.isMember("mode") || !NsAppLinkRPC::ButtonPressModeMarshaller::fromJSON(js["mode"],c.mode))  return false;

    if(c.customButtonName)  delete c.customButtonName;
    c.customButtonName=0;
    if(js.isMember("customButtonName"))
    {
      if(!js["customButtonName"].isString())  return false;
      c.customButtonName=new std::string();
      c.customButtonName[0]=js["customButtonName"].asString();
      if(c.customButtonName[0].length()>500)  return false;

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
