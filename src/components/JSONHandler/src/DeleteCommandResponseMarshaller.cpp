#include "JSONHandler/DeleteCommandResponseMarshaller.h"
#include <json/reader.h>
#include <json/writer.h>
#include "ALRPCObjectsImpl/ResultMarshaller.h"

using namespace RPC2Communication;

bool DeleteCommandResponseMarshaller::fromString(const std::string& s,DeleteCommandResponse& e)
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


const std::string DeleteCommandResponseMarshaller::toString(const DeleteCommandResponse& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value DeleteCommandResponseMarshaller::toJSON(const DeleteCommandResponse& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  
  json["result"] = ResultMarshaller::toJSON(e.getResult());

  //TODO: error marshaller
  json["id"] = e.getID();

  return json;
}


bool DeleteCommandResponseMarshaller::fromJSON(const Json::Value& json,DeleteCommandResponse& c)
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

