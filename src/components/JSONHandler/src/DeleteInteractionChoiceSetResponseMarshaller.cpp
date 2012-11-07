#include "JSONHandler/DeleteInteractionChoiceSetResponseMarshaller.h"
#include <json/reader.h>
#include <json/writer.h>
#include "ALRPCObjectsImpl/ResultMarshaller.h"

using namespace RPC2Communication;

bool DeleteInteractionChoiceSetResponseMarshaller::fromString(const std::string& s,DeleteInteractionChoiceSetResponse& e)
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


const std::string DeleteInteractionChoiceSetResponseMarshaller::toString(const DeleteInteractionChoiceSetResponse& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value DeleteInteractionChoiceSetResponseMarshaller::toJSON(const DeleteInteractionChoiceSetResponse& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  
  json["result"] = ResultMarshaller::toJSON(e.getResult());

  //TODO: error marshaller
  json["id"] = e.getID();

  return json;
}


bool DeleteInteractionChoiceSetResponseMarshaller::fromJSON(const Json::Value& json,DeleteInteractionChoiceSetResponse& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(json.isMember("result"))
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

