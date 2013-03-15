//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ShowConstantTBT.h"
#include "../src/SDLRPCObjectsImpl/V2/ImageMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/SoftButtonMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ShowConstantTBTMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;

bool ShowConstantTBTMarshaller::checkIntegrity(ShowConstantTBT& s)
{
  return checkIntegrityConst(s);
}


bool ShowConstantTBTMarshaller::fromString(const std::string& s,ShowConstantTBT& e)
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


const std::string ShowConstantTBTMarshaller::toString(const ShowConstantTBT& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ShowConstantTBTMarshaller::checkIntegrityConst(const ShowConstantTBT& s)
{
  if(s.navigationText1 && (s.navigationText1[0].length()>500))  return false;

  if(s.navigationText2 && (s.navigationText2[0].length()>500))  return false;

  if(s.eta && (s.eta[0].length()>500))  return false;

  if(s.totalDistance && (s.totalDistance[0].length()>500))  return false;

  if(!NsSmartDeviceLinkRPCV2::ImageMarshaller::checkIntegrityConst(s.turnIcon))  return false;

  if(s.distanceToManeuver<0 || s.distanceToManeuver>1000000000)  return false;

  if(s.distanceToManeuverScale<0 || s.distanceToManeuverScale>1000000000)  return false;

  {
    unsigned int i=s.softButtons.size();
    if(i<0)  return false;
    if(i>3)  return false;
  }

  return true;
}


Json::Value ShowConstantTBTMarshaller::toJSON(const ShowConstantTBT& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.ShowConstantTBT");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  if(e.navigationText1)
    json["params"]["navigationText1"]=Json::Value(e.navigationText1[0]);;
  if(e.navigationText2)
    json["params"]["navigationText2"]=Json::Value(e.navigationText2[0]);;
  if(e.eta)
    json["params"]["eta"]=Json::Value(e.eta[0]);;
  if(e.totalDistance)
    json["params"]["totalDistance"]=Json::Value(e.totalDistance[0]);;
  json["params"]["turnIcon"]=NsSmartDeviceLinkRPCV2::ImageMarshaller::toJSON(e.turnIcon);;
  json["params"]["distanceToManeuver"]=Json::Value(e.distanceToManeuver);;
  json["params"]["distanceToManeuverScale"]=Json::Value(e.distanceToManeuverScale);;
  if(e.maneuverComplete)
    json["params"]["maneuverComplete"]=Json::Value(e.maneuverComplete[0]);;
  {
    unsigned int i=e.softButtons.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsSmartDeviceLinkRPCV2::SoftButtonMarshaller::toJSON(e.softButtons[i]);

    json["params"]["softButtons"]=j;
  }
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool ShowConstantTBTMarshaller::fromJSON(const Json::Value& json,ShowConstantTBT& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.ShowConstantTBT"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(c.navigationText1)  delete c.navigationText1;
    c.navigationText1=0;
    if(js.isMember("navigationText1"))
    {
      if(!js["navigationText1"].isString())  return false;
      c.navigationText1=new std::string();
      c.navigationText1[0]=js["navigationText1"].asString();
      if(c.navigationText1[0].length()>500)  return false;

    }

    if(c.navigationText2)  delete c.navigationText2;
    c.navigationText2=0;
    if(js.isMember("navigationText2"))
    {
      if(!js["navigationText2"].isString())  return false;
      c.navigationText2=new std::string();
      c.navigationText2[0]=js["navigationText2"].asString();
      if(c.navigationText2[0].length()>500)  return false;

    }

    if(c.eta)  delete c.eta;
    c.eta=0;
    if(js.isMember("eta"))
    {
      if(!js["eta"].isString())  return false;
      c.eta=new std::string();
      c.eta[0]=js["eta"].asString();
      if(c.eta[0].length()>500)  return false;

    }

    if(c.totalDistance)  delete c.totalDistance;
    c.totalDistance=0;
    if(js.isMember("totalDistance"))
    {
      if(!js["totalDistance"].isString())  return false;
      c.totalDistance=new std::string();
      c.totalDistance[0]=js["totalDistance"].asString();
      if(c.totalDistance[0].length()>500)  return false;

    }

    if(!js.isMember("turnIcon") || !NsSmartDeviceLinkRPCV2::ImageMarshaller::fromJSON(js["turnIcon"],c.turnIcon))  return false;

    if(!js.isMember("distanceToManeuver") || !js["distanceToManeuver"].isNumeric())  return false;
    c.distanceToManeuver=js["distanceToManeuver"].asDouble();
    if(c.distanceToManeuver<0 || c.distanceToManeuver>1000000000)  return false;

    if(!js.isMember("distanceToManeuverScale") || !js["distanceToManeuverScale"].isNumeric())  return false;
    c.distanceToManeuverScale=js["distanceToManeuverScale"].asDouble();
    if(c.distanceToManeuverScale<0 || c.distanceToManeuverScale>1000000000)  return false;

    if(c.maneuverComplete)  delete c.maneuverComplete;
    c.maneuverComplete=0;
    if(js.isMember("maneuverComplete"))
    {
      if(!js["maneuverComplete"].isBool())  return false;
      c.maneuverComplete=new bool();
      c.maneuverComplete[0]=js["maneuverComplete"].asBool();
      
    }

    if(!js.isMember("softButtons") || !js["softButtons"].isArray())  return false;
    {
      unsigned int i=js["softButtons"].size();
      if(i<0)  return false;
      if(i>3)  return false;
      c.softButtons.resize(i);
      while(i--)
      {
        NsSmartDeviceLinkRPCV2::SoftButton t;
        if(!NsSmartDeviceLinkRPCV2::SoftButtonMarshaller::fromJSON(js["softButtons"][i],t))
          return false;
         c.softButtons[i]=t;
      }
    }

    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
