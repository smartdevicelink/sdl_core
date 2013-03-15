//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/Slider_request.h"


#include "Slider_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


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
  if(s.numTicks>26)  return false;
  if(s.numTicks<2)  return false;
  if(s.position>26)  return false;
  if(s.position<1)  return false;
  if(s.sliderHeader.length()>500)  return false;
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

  json["numTicks"]=Json::Value(e.numTicks);

  json["position"]=Json::Value(e.position);

  json["sliderHeader"]=Json::Value(e.sliderHeader);

  if(e.sliderFooter)
  {
    unsigned int sz=e.sliderFooter->size();
    json["sliderFooter"]=Json::Value(Json::arrayValue);
    json["sliderFooter"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["sliderFooter"][i]=Json::Value(e.sliderFooter[0][i]);
  }

  json["timeout"]=Json::Value(e.timeout);

  return json;
}


bool Slider_requestMarshaller::fromJSON(const Json::Value& json,Slider_request& c)
{
  if(c.sliderFooter)  delete c.sliderFooter;
  c.sliderFooter=0;

  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("numTicks"))  return false;
    {
      const Json::Value& j=json["numTicks"];
      if(!j.isInt())  return false;
      c.numTicks=j.asInt();
    }
    if(!json.isMember("position"))  return false;
    {
      const Json::Value& j=json["position"];
      if(!j.isInt())  return false;
      c.position=j.asInt();
    }
    if(!json.isMember("sliderHeader"))  return false;
    {
      const Json::Value& j=json["sliderHeader"];
      if(!j.isString())  return false;
      c.sliderHeader=j.asString();
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

