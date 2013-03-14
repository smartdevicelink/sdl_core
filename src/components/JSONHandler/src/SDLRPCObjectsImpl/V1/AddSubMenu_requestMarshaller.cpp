#include "../include/JSONHandler/SDLRPCObjects/V1/AddSubMenu_request.h"


#include "AddSubMenu_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

using namespace NsSmartDeviceLinkRPC;


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

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("AddSubMenu");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["menuID"]=Json::Value(e.menuID);

  if(e.position)
    j["position"]=Json::Value(*e.position);

  j["menuName"]=Json::Value(e.menuName);

  json["request"]["parameters"]=j;
  return json;
}


bool AddSubMenu_requestMarshaller::fromJSON(const Json::Value& js,AddSubMenu_request& c)
{
  if(c.position)  delete c.position;
  c.position=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("AddSubMenu"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
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

