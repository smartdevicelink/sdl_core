#include "../../include/JSONHandler/ALRPCObjects/MenuParams.h"


#include "MenuParamsMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


bool MenuParamsMarshaller::checkIntegrity(MenuParams& s)
{
  return checkIntegrityConst(s);
}


bool MenuParamsMarshaller::fromString(const std::string& s,MenuParams& e)
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


const std::string MenuParamsMarshaller::toString(const MenuParams& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool MenuParamsMarshaller::checkIntegrityConst(const MenuParams& s)
{
  if(s.menuName.length()>500)  return false;
  if(s.parentID && *s.parentID>2000000000)  return false;
  if(s.position && *s.position>1000)  return false;
  return true;
}

Json::Value MenuParamsMarshaller::toJSON(const MenuParams& e)
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


bool MenuParamsMarshaller::fromJSON(const Json::Value& json,MenuParams& c)
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

