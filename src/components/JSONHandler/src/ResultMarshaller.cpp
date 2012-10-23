#include "JSONHandler/ResultMarshaller.h"

#include <json/reader.h>
#include <json/writer.h>

bool ResultMarshaller::fromJSON(const Json::Value& s,Result& e)
{
  if(s.isNull())
    return false;

  e.mInternalResult = s.asString();

  return true;
}


Json::Value ResultMarshaller::toJSON(const Result& e)
{
  Json::Value json(e.mInternalResult);
   
  return json;
}


bool ResultMarshaller::fromString(const std::string& s,Result& e)
{
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(fromJSON(json,e))  return true;
  }
  catch(...)
  {
    return false;
  }
  return false;
}

const std::string ResultMarshaller::toString(const Result& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}
