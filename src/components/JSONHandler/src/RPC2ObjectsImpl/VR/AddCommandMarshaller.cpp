#include "../include/JSONHandler/RPC2Objects/VR/AddCommand.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "AddCommandMarshaller.h"

/*
  interface	RPC2Communication::VR
  version	1.2
  generated at	Wed Nov  7 11:26:04 2012
  source stamp	Wed Nov  7 09:31:14 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace RPC2Communication::VR;

bool AddCommandMarshaller::checkIntegrity(AddCommand& s)
{
  return checkIntegrityConst(s);
}


bool AddCommandMarshaller::fromString(const std::string& s,AddCommand& e)
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


const std::string AddCommandMarshaller::toString(const AddCommand& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool AddCommandMarshaller::checkIntegrityConst(const AddCommand& s)
{
  if(s.cmdId>2000000000)  return false;

  {
    unsigned int i=s.vrCommands.size();
    if(i<1)  return false;
    if(i>100)  return false;
    while(i--)
    {
      if(s.vrCommands[i].length()>99)  return false;
    }
  }

  return true;
}


Json::Value AddCommandMarshaller::toJSON(const AddCommand& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("VR.AddCommand");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["cmdId"]=Json::Value(e.cmdId);;
  {
    unsigned int i=e.vrCommands.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=Json::Value(e.vrCommands[i]);

    json["params"]["vrCommands"]=j;
  }
  return json;
}


bool AddCommandMarshaller::fromJSON(const Json::Value& json,AddCommand& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("VR.AddCommand"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("cmdId") || !js["cmdId"].isInt())  return false;
    c.cmdId=js["cmdId"].asInt();
    if(c.cmdId>2000000000)  return false;

    if(!js.isMember("vrCommands") || !js["vrCommands"].isArray())
      return false;
    {
      c.vrCommands.clear();
      unsigned int i=js["vrCommands"].size();
      if(i<1)  return false;
      if(i>100)  return false;
      c.vrCommands.resize(i);
      while(i--)
      {
        if(!js["vrCommands"][i].isString())
          return false;
        c.vrCommands[i]=js["vrCommands"][i].asString();
        if(c.vrCommands[i].length()>99)  return false;

      }
    }


  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
