#include "../include/JSONHandler/ALRPCObjects/AddCommand_v2_request.h"
#include "ImageMarshaller.h"
#include "MenuParams_v2Marshaller.h"

#include "AddCommand_v2_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool AddCommand_v2_requestMarshaller::checkIntegrity(AddCommand_v2_request& s)
{
  return checkIntegrityConst(s);
}


bool AddCommand_v2_requestMarshaller::fromString(const std::string& s,AddCommand_v2_request& e)
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


const std::string AddCommand_v2_requestMarshaller::toString(const AddCommand_v2_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool AddCommand_v2_requestMarshaller::checkIntegrityConst(const AddCommand_v2_request& s)
{
  if(s.cmdID && *s.cmdID>2000000000)  return false;
  if(s.menuParams && !MenuParams_v2Marshaller::checkIntegrityConst(*s.menuParams))  return false;
  if(s.vrCommands)
  {
    unsigned int i=s.vrCommands[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
      if(s.vrCommands[0][i].length()>99)  return false;
    }
  }
  if(s.cmdIcon && !ImageMarshaller::checkIntegrityConst(*s.cmdIcon))  return false;
  return true;
}

Json::Value AddCommand_v2_requestMarshaller::toJSON(const AddCommand_v2_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("AddCommand_v2");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  if(e.cmdID)
    j["cmdID"]=Json::Value(*e.cmdID);

  if(e.menuParams)
    j["menuParams"]=MenuParams_v2Marshaller::toJSON(*e.menuParams);

  if(e.vrCommands)
  {
    unsigned int sz=e.vrCommands->size();
    j["vrCommands"]=Json::Value(Json::arrayValue);
    j["vrCommands"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["vrCommands"][i]=Json::Value(e.vrCommands[0][i]);
  }

  if(e.cmdIcon)
    j["cmdIcon"]=ImageMarshaller::toJSON(*e.cmdIcon);

  json["request"]["parameters"]=j;
  return json;
}


bool AddCommand_v2_requestMarshaller::fromJSON(const Json::Value& js,AddCommand_v2_request& c)
{
  if(c.cmdID)  delete c.cmdID;
  c.cmdID=0;

  if(c.menuParams)  delete c.menuParams;
  c.menuParams=0;

  if(c.vrCommands)  delete c.vrCommands;
  c.vrCommands=0;

  if(c.cmdIcon)  delete c.cmdIcon;
  c.cmdIcon=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("AddCommand_v2"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(json.isMember("cmdID"))
    {
      const Json::Value& j=json["cmdID"];
      if(!j.isInt())  return false;
      c.cmdID=new unsigned int(j.asInt());
    }
    if(json.isMember("menuParams"))
    {
      const Json::Value& j=json["menuParams"];
      c.menuParams=new MenuParams_v2();
      if(!MenuParams_v2Marshaller::fromJSON(j,c.menuParams[0]))
        return false;
    }
    if(json.isMember("vrCommands"))
    {
      const Json::Value& j=json["vrCommands"];
      if(!j.isArray())  return false;
      c.vrCommands=new std::vector<std::string>();
      c.vrCommands->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        if(!j[i].isString())
          return false;
        else
          c.vrCommands[0][i]=j[i].asString();
    }
    if(json.isMember("cmdIcon"))
    {
      const Json::Value& j=json["cmdIcon"];
      c.cmdIcon=new Image();
      if(!ImageMarshaller::fromJSON(j,c.cmdIcon[0]))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

