#include "../include/JSONHandler/ALRPCObjects/MenuParams_v2.h"


#include "MenuParams_v2Marshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool MenuParams_v2Marshaller::checkIntegrity(MenuParams_v2& s)
{
  return checkIntegrityConst(s);
}


bool MenuParams_v2Marshaller::fromString(const std::string& s,MenuParams_v2& e)
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


const std::string MenuParams_v2Marshaller::toString(const MenuParams_v2& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool MenuParams_v2Marshaller::checkIntegrityConst(const MenuParams_v2& s)
{
  if(s.menuName.length()>500)  return false;
  if(s.parentID && *s.parentID>2000000000)  return false;
  if(s.position && *s.position>1000)  return false;
  return true;
}

Json::Value MenuParams_v2Marshaller::toJSON(const MenuParams_v2& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["menuName"]=Json::Value(e.menuName);

  if(e.parentID)
    json["parentID"]=Json::Value(*e.parentID);

  if(e.position)
    json["position"]=Json::Value(*e.position);


  return json;
}


bool MenuParams_v2Marshaller::fromJSON(const Json::Value& json,MenuParams_v2& c)
{
  if(c.parentID)  delete c.parentID;
  c.parentID=0;

  if(c.position)  delete c.position;
  c.position=0;

  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("menuName"))  return false;
    {
      const Json::Value& j=json["menuName"];
      if(!j.isString())  return false;
      c.menuName=j.asString();
    }
    if(json.isMember("parentID"))
    {
      const Json::Value& j=json["parentID"];
      if(!j.isInt())  return false;
      c.parentID=new unsigned int(j.asInt());
    }
    if(json.isMember("position"))
    {
      const Json::Value& j=json["position"];
      if(!j.isInt())  return false;
      c.position=new unsigned int(j.asInt());
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

