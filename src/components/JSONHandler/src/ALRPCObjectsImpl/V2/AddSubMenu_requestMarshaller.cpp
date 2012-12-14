#include "../include/JSONHandler/ALRPCObjects/V2/AddSubMenu_request.h"


#include "AddSubMenu_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool AddSubMenu_requestMarshaller::checkIntegrity(AddSubMenu_request& s)
{
  return checkIntegrityConst(s);
}


bool AddSubMenu_requestMarshaller::fromString(const std::string& s,AddSubMenu_request& e)
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


const std::string AddSubMenu_requestMarshaller::toString(const AddSubMenu_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool AddSubMenu_requestMarshaller::checkIntegrityConst(const AddSubMenu_request& s)
{
  if(s.menuID>2000000000)  return false;
  if(s.position && *s.position>1000)  return false;
  if(s.menuName.length()>500)  return false;
  return true;
}

Json::Value AddSubMenu_requestMarshaller::toJSON(const AddSubMenu_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["menuID"]=Json::Value(e.menuID);

  if(e.position)
    json["position"]=Json::Value(*e.position);

  json["menuName"]=Json::Value(e.menuName);

  return json;
}


bool AddSubMenu_requestMarshaller::fromJSON(const Json::Value& json,AddSubMenu_request& c)
{
  if(c.position)  delete c.position;
  c.position=0;

  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("menuID"))  return false;
    {
      const Json::Value& j=json["menuID"];
      if(!j.isInt())  return false;
      c.menuID=j.asInt();
    }
    if(json.isMember("position"))
    {
      const Json::Value& j=json["position"];
      if(!j.isInt())  return false;
      c.position=new unsigned int(j.asInt());
    }
    if(!json.isMember("menuName"))  return false;
    {
      const Json::Value& j=json["menuName"];
      if(!j.isString())  return false;
      c.menuName=j.asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

