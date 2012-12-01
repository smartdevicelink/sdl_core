#include "../include/JSONHandler/ALRPCObjects/Slider_request.h"


#include "Slider_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool Slider_requestMarshaller::checkIntegrity(Slider_request& s)
{
  return checkIntegrityConst(s);
}


bool Slider_requestMarshaller::fromString(const std::string& s,Slider_request& e)
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


const std::string Slider_requestMarshaller::toString(const Slider_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool Slider_requestMarshaller::checkIntegrityConst(const Slider_request& s)
{
  if(s.numTicks && *s.numTicks>26)  return false;
  if(s.numTicks && *s.numTicks<2)  return false;
  if(s.position>26)  return false;
  if(s.position<1)  return false;
  if(s.sliderHeader && s.sliderHeader->length()>500)  return false;
  if(s.sliderFooter)
  {
    unsigned int i=s.sliderFooter[0].size();
    if(i>26 || i<1)  return false;
    while(i--)
    {
      if(s.sliderFooter[0][i].length()>500)  return false;
    }
  }
  if(s.timeout>65535)  return false;
  return true;
}

Json::Value Slider_requestMarshaller::toJSON(const Slider_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("Slider");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  if(e.numTicks)
    j["numTicks"]=Json::Value(*e.numTicks);

  j["position"]=Json::Value(e.position);

  if(e.sliderHeader)
    j["sliderHeader"]=Json::Value(*e.sliderHeader);

  if(e.sliderFooter)
  {
    unsigned int sz=e.sliderFooter->size();
    j["sliderFooter"]=Json::Value(Json::arrayValue);
    j["sliderFooter"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["sliderFooter"][i]=Json::Value(e.sliderFooter[0][i]);
  }

  j["timeout"]=Json::Value(e.timeout);

  json["request"]["parameters"]=j;
  return json;
}


bool Slider_requestMarshaller::fromJSON(const Json::Value& js,Slider_request& c)
{
  if(c.numTicks)  delete c.numTicks;
  c.numTicks=0;

  if(c.sliderHeader)  delete c.sliderHeader;
  c.sliderHeader=0;

  if(c.sliderFooter)  delete c.sliderFooter;
  c.sliderFooter=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("Slider"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(json.isMember("numTicks"))
    {
      const Json::Value& j=json["numTicks"];
      if(!j.isInt())  return false;
      c.numTicks=new unsigned int(j.asInt());
    }
    if(!json.isMember("position"))  return false;
    {
      const Json::Value& j=json["position"];
      if(!j.isInt())  return false;
      c.position=j.asInt();
    }
    if(json.isMember("sliderHeader"))
    {
      const Json::Value& j=json["sliderHeader"];
      if(!j.isString())  return false;
      c.sliderHeader=new std::string(j.asString());
    }
    if(json.isMember("sliderFooter"))
    {
      const Json::Value& j=json["sliderFooter"];
      if(!j.isArray())  return false;
      c.sliderFooter=new std::vector<std::string>();
      c.sliderFooter->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        if(!j[i].isString())
          return false;
        else
          c.sliderFooter[0][i]=j[i].asString();
    }
    if(!json.isMember("timeout"))  return false;
    {
      const Json::Value& j=json["timeout"];
      if(!j.isInt())  return false;
      c.timeout=j.asInt();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

