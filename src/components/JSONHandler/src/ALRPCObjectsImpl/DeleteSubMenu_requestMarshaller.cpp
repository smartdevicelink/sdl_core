#include "../../include/JSONHandler/ALRPCObjects/DeleteSubMenu_request.h"


#include "DeleteSubMenu_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Oct 30 08:29:32 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


bool DeleteSubMenu_requestMarshaller::checkIntegrity(DeleteSubMenu_request& s)
{
  return checkIntegrityConst(s);
}


bool DeleteSubMenu_requestMarshaller::fromString(const std::string& s,DeleteSubMenu_request& e)
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


const std::string DeleteSubMenu_requestMarshaller::toString(const DeleteSubMenu_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool DeleteSubMenu_requestMarshaller::checkIntegrityConst(const DeleteSubMenu_request& s)
{
  if(s.menuID>2000000000)  return false;
  return true;
}

Json::Value DeleteSubMenu_requestMarshaller::toJSON(const DeleteSubMenu_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("DeleteSubMenu");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["menuID"]=Json::Value(e.menuID);

  json["request"]["parameters"]=j;
  return json;
}


bool DeleteSubMenu_requestMarshaller::fromJSON(const Json::Value& js,DeleteSubMenu_request& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("DeleteSubMenu"))  return false;
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

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

