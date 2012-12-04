#include "../include/JSONHandler/ALRPCObjects/V2/DeleteFile_response.h"
#include "ResultMarshaller.h"

#include "DeleteFile_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool DeleteFile_responseMarshaller::checkIntegrity(DeleteFile_response& s)
{
  return checkIntegrityConst(s);
}


bool DeleteFile_responseMarshaller::fromString(const std::string& s,DeleteFile_response& e)
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


const std::string DeleteFile_responseMarshaller::toString(const DeleteFile_response& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool DeleteFile_responseMarshaller::checkIntegrityConst(const DeleteFile_response& s)
{
  if(!ResultMarshaller::checkIntegrityConst(s.resultCode))  return false;
  if(s.spaceAvailable>2000000000)  return false;
  if(s.info && s.info->length()>1000)  return false;
  return true;
}

Json::Value DeleteFile_responseMarshaller::toJSON(const DeleteFile_response& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["success"]=Json::Value(e.success);

  json["resultCode"]=ResultMarshaller::toJSON(e.resultCode);

  json["spaceAvailable"]=Json::Value(e.spaceAvailable);

  if(e.info)
    json["info"]=Json::Value(*e.info);

  return json;
}


bool DeleteFile_responseMarshaller::fromJSON(const Json::Value& json,DeleteFile_response& c)
{
  if(c.info)  delete c.info;
  c.info=0;

  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("success"))  return false;
    {
      const Json::Value& j=json["success"];
      if(!j.isBool())  return false;
      c.success=j.asBool();
    }
    if(!json.isMember("resultCode"))  return false;
    {
      const Json::Value& j=json["resultCode"];
      if(!ResultMarshaller::fromJSON(j,c.resultCode))
        return false;
    }
    if(!json.isMember("spaceAvailable"))  return false;
    {
      const Json::Value& j=json["spaceAvailable"];
      if(!j.isInt())  return false;
      c.spaceAvailable=j.asInt();
    }
    if(json.isMember("info"))
    {
      const Json::Value& j=json["info"];
      if(!j.isString())  return false;
      c.info=new std::string(j.asString());
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

