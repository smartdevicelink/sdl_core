#include "../include/JSONHandler/ALRPCObjects/V2/DeleteSubMenu_request.h"


#include "DeleteSubMenu_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


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

  json["menuID"]=Json::Value(e.menuID);

  return json;
}


bool DeleteSubMenu_requestMarshaller::fromJSON(const Json::Value& json,DeleteSubMenu_request& c)
{
  try
  {
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

