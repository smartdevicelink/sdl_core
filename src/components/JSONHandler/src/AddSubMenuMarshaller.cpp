#include "JSONHandler/AddSubMenuMarshaller.h"
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

using namespace RPC2Communication;

bool AddSubMenuMarshaller::fromString(const std::string& s,AddSubMenu& e)
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


const std::string AddSubMenuMarshaller::toString(const AddSubMenu& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value AddSubMenuMarshaller::toJSON(const AddSubMenu& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.AddSubMenu");
  
  Json::Value j=Json::Value(Json::objectValue);
  j["menuId"]=e.menuId; 

  if (e.position)
  {
    j["position"] = *e.position;
  }

  j["menuName"] = e.menuName;
      
  json["params"]=j;

  json["id"] = e.getID();

  return json;
}


bool AddSubMenuMarshaller::fromJSON(const Json::Value& json,AddSubMenu& c)
{
  delete c.position;

  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if(json["method"].asString().compare("UI.AddSubMenu"))
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if(!j.isMember("menuId") || !j["menuId"].isString()) return false;
    c.menuId = j["menuId"].asInt();
    
    if(j.isMember("position"))
    {
      if(!j["position"].isInt()) 
        return false;
      c.position = new int(j["position"].asInt());
    }     

    if(!j.isMember("menuName") || !j["menuName"].isString()) return false;
    c.menuName = j["menuName"].asString();

    if(!json.isMember("id")) return false;

    c.setID( json["id"].asInt() );
  } 
  catch(...)
  {
    return false;
  }

  return true;
}

