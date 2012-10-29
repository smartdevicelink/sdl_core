#include "JSONHandler/AlertMarshaller.h"
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

using namespace RPC2Communication;

bool AlertMarshaller::fromString(const std::string& s,Alert& e)
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


const std::string AlertMarshaller::toString(const Alert& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value AlertMarshaller::toJSON(const Alert& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.Alert");
  
  Json::Value j=Json::Value(Json::objectValue);
  if (e.mAlertText1)
  {
    j["AlertText1"]=e.mAlertText1;  
  }

  if (e.mAlertText2)
  {
    j["AlertText2"] = e.mAlertText2;
  }
  
  if (e.mDuration)
  {
    j["duration"] = e.mDuration;
  }

  if (e.mPlayTone)
  {
    j["playTone"] = e.mPlayTone;
  } 
    
  json["params"]=j;

  json["id"] = e.getID();

  return json;
}


bool AlertMarshaller::fromJSON(const Json::Value& json,Alert& c)
{
  delete c.mAlertText1;
  delete c.mAlertText2;
  delete c.mDuration;
  delete c.mPlayTone;

  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if (json["method"].asString().compare("UI.Alert"))  
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if(j.isMember("AlertText1"))
    {
      if (!j["AlertText1"].isString())
        return false;
      c.mAlertText1 = new std::string(j["AlertText1"].asString());
    }    

    if(j.isMember("AlertText2"))
    {
      if(!j["AlertText2"].isString()) 
        return false;
      c.mAlertText2 = new std::string(j["AlertText2"].asString());
    }     

    if(j.isMember("duration"))
    {
      if (!j["duration"].isInt())
        return false;
      c.mDuration = new int(j["duration"].asInt());
    }     

    if(j.isMember("playTone"))
    {
      if (!j["playTone"].isBool())
        return false;
      c.mPlayTone = new bool( j["playTone"].asBool());
    }     

    if(!json.isMember("id")) return false;

    c.setID( json["id"].asInt() );
  } 
  catch(...)
  {
    return false;
  }

  return true;
}

