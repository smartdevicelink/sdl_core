#include "../include/JSONHandler/ALRPCObjects/V2/VrHelpItem.h"
#include "ImageMarshaller.h"

#include "VrHelpItemMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool VrHelpItemMarshaller::checkIntegrity(VrHelpItem& s)
{
  return checkIntegrityConst(s);
}


bool VrHelpItemMarshaller::fromString(const std::string& s,VrHelpItem& e)
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


const std::string VrHelpItemMarshaller::toString(const VrHelpItem& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool VrHelpItemMarshaller::checkIntegrityConst(const VrHelpItem& s)
{
  if(!ImageMarshaller::checkIntegrityConst(s.image))  return false;
  if(s.position>8)  return false;
  if(s.position<1)  return false;
  if(s.text.length()>500)  return false;
  return true;
}

Json::Value VrHelpItemMarshaller::toJSON(const VrHelpItem& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["image"]=ImageMarshaller::toJSON(e.image);

  json["position"]=Json::Value(e.position);

  json["text"]=Json::Value(e.text);


  return json;
}


bool VrHelpItemMarshaller::fromJSON(const Json::Value& json,VrHelpItem& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("image"))  return false;
    {
      const Json::Value& j=json["image"];
      if(!ImageMarshaller::fromJSON(j,c.image))
        return false;
    }
    if(!json.isMember("position"))  return false;
    {
      const Json::Value& j=json["position"];
      if(!j.isInt())  return false;
      c.position=j.asInt();
    }
    if(!json.isMember("text"))  return false;
    {
      const Json::Value& j=json["text"];
      if(!j.isString())  return false;
      c.text=j.asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

