#include "JSONHandler/DeleteSubMenuMarshaller.h"
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

using namespace RPC2Communication;

bool DeleteSubMenuMarshaller::fromString(const std::string& s,DeleteSubMenu& e)
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


const std::string DeleteSubMenuMarshaller::toString(const DeleteSubMenu& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value DeleteSubMenuMarshaller::toJSON(const DeleteSubMenu& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.DeleteSubMenu");
  
  Json::Value j=Json::Value(Json::objectValue);
  j["menuId"] = e.menuId;
    
  json["params"]=j;

  json["id"] = e.getID();

  return json;
}


bool DeleteSubMenuMarshaller::fromJSON(const Json::Value& json,DeleteSubMenu& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if (json["method"].asString().compare("UI.DeleteSubMenu"))  
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if(!j.isMember("menuId") || !j["menuId"].isInt()) return false;
    c.menuId = j["menuId"].asInt();

    if(!json.isMember("id")) return false;

    c.setID( json["id"].asInt() );
  } 
  catch(...)
  {
    return false;
  }

  return true;
}

