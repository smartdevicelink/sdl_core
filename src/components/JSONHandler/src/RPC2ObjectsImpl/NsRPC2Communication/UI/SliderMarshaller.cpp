#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/Slider.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SliderMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool SliderMarshaller::checkIntegrity(Slider& s)
{
  return checkIntegrityConst(s);
}


bool SliderMarshaller::fromString(const std::string& s,Slider& e)
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


const std::string SliderMarshaller::toString(const Slider& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SliderMarshaller::checkIntegrityConst(const Slider& s)
{
  if(s.numTicks<2 || s.numTicks>26)  return false;

  if(s.position<1 || s.position>16)  return false;

  if(s.sliderHeader.length()>500)  return false;

  if(s.sliderFooter)
  {
    unsigned int i=s.sliderFooter[0].size();
    if(i<1)  return false;
    if(i>26)  return false;
    while(i--)
    {
      if(s.sliderFooter[0][i].length()>500)  return false;
    }
  }

  if(s.timeout>65535)  return false;

  return true;
}


Json::Value SliderMarshaller::toJSON(const Slider& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.Slider");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["numTicks"]=Json::Value(e.numTicks);;
  json["params"]["position"]=Json::Value(e.position);;
  json["params"]["sliderHeader"]=Json::Value(e.sliderHeader);;
  if(e.sliderFooter)
  {
    unsigned int i=e.sliderFooter[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=Json::Value(e.sliderFooter[0][i]);

    json["params"]["sliderFooter"]=j;
  }
  json["params"]["timeout"]=Json::Value(e.timeout);;
  return json;
}


bool SliderMarshaller::fromJSON(const Json::Value& json,Slider& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.Slider"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("numTicks") || !js["numTicks"].isInt())  return false;
    c.numTicks=js["numTicks"].asInt();
    if(c.numTicks<2 || c.numTicks>26)  return false;

    if(!js.isMember("position") || !js["position"].isInt())  return false;
    c.position=js["position"].asInt();
    if(c.position<1 || c.position>16)  return false;

    if(!js.isMember("sliderHeader") || !js["sliderHeader"].isString())  return false;
    c.sliderHeader=js["sliderHeader"].asString();
    if(c.sliderHeader.length()>500)  return false;

    if(c.sliderFooter)  delete c.sliderFooter;
    c.sliderFooter=0;
    if(js.isMember("sliderFooter"))
    {
      if(!js["sliderFooter"].isArray()) return false;
      unsigned int i=js["sliderFooter"].size();
      if(i<1)  return false;
      if(i>26)  return false;

      c.sliderFooter=new std::vector<std::string>();
      c.sliderFooter->resize(js["sliderFooter"].size());

      while(i--)
      {
        if(!js["sliderFooter"][i].isString())
          return false;

        c.sliderFooter[0][i]=js["sliderFooter"][i].asString();
        if(c.sliderFooter[0][i].length()>500)  return false;
      }
    }


    if(!js.isMember("timeout") || !js["timeout"].isInt())  return false;
    c.timeout=js["timeout"].asInt();
    if(c.timeout>65535)  return false;

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
