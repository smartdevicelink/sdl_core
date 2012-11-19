#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/Alert.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "AlertMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 10:37:39 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool AlertMarshaller::checkIntegrity(Alert& s)
{
  return checkIntegrityConst(s);
}


bool AlertMarshaller::fromString(const std::string& s,Alert& e)
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


const std::string AlertMarshaller::toString(const Alert& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool AlertMarshaller::checkIntegrityConst(const Alert& s)
{
  if(s.AlertText1 && (s.AlertText1[0].length()>500))  return false;

  if(s.AlertText2 && (s.AlertText2[0].length()>500))  return false;

  if(s.duration && (s.duration[0]<3000 || s.duration[0]>10000))  return false;

  return true;
}


Json::Value AlertMarshaller::toJSON(const Alert& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.Alert");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  if(e.AlertText1)
    json["params"]["AlertText1"]=Json::Value(e.AlertText1[0]);;
  if(e.AlertText2)
    json["params"]["AlertText2"]=Json::Value(e.AlertText2[0]);;
  if(e.duration)
    json["params"]["duration"]=Json::Value(e.duration[0]);;
  if(e.playTone)
    json["params"]["playTone"]=Json::Value(e.playTone[0]);;
  return json;
}


bool AlertMarshaller::fromJSON(const Json::Value& json,Alert& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.Alert"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(c.AlertText1)  delete c.AlertText1;
    c.AlertText1=0;
    if(js.isMember("AlertText1"))
    {
      if(!js["AlertText1"].isString())  return false;
      c.AlertText1=new std::string();
      c.AlertText1[0]=js["AlertText1"].asString();
      if(c.AlertText1[0].length()>500)  return false;

    }

    if(c.AlertText2)  delete c.AlertText2;
    c.AlertText2=0;
    if(js.isMember("AlertText2"))
    {
      if(!js["AlertText2"].isString())  return false;
      c.AlertText2=new std::string();
      c.AlertText2[0]=js["AlertText2"].asString();
      if(c.AlertText2[0].length()>500)  return false;

    }

    if(c.duration)  delete c.duration;
    c.duration=0;
    if(js.isMember("duration"))
    {
      if(!js["duration"].isInt())  return false;
      c.duration=new unsigned int();
      c.duration[0]=js["duration"].asInt();
      if(c.duration[0]<3000 || c.duration[0]>10000)  return false;

    }

    if(c.playTone)  delete c.playTone;
    c.playTone=0;
    if(js.isMember("playTone"))
    {
      if(!js["playTone"].isBool())  return false;
      c.playTone=new bool();
      c.playTone[0]=js["playTone"].asBool();
      
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
