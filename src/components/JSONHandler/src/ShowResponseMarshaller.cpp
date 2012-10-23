#include "JSONHandler/ShowResponseMarshaller.h"
#include <json/reader.h>
#include <json/writer.h>
#include "JSONHandler/ResultMarshaller.h"

using namespace RPC2Communication;

bool ShowResponseMarshaller::fromString(const std::string& s,ShowResponse& e)
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


const std::string ShowResponseMarshaller::toString(const ShowResponse& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value ShowResponseMarshaller::toJSON(const ShowResponse& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value(Json::objectValue);
  
  json["result"] = ResultMarshaller::toJSON(e.getResult());

  //TODO: error marshaller
  json["id"] = e.getID();

  return json;
}


bool ShowResponseMarshaller::fromJSON(const Json::Value& json,ShowResponse& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("result"))
    {
        Result r;
        if(!ResultMarshaller::fromJSON(json["result"],r)) return false;
        c.setResult(r);
    }
    else
    {
        if (!json.isMember("error"))
            return false;
        //TODO: error marshaller
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
