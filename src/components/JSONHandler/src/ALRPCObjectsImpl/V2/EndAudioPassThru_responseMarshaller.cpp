#include "../include/JSONHandler/ALRPCObjects/V2/EndAudioPassThru_response.h"
#include "ResultMarshaller.h"

#include "EndAudioPassThru_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool EndAudioPassThru_responseMarshaller::checkIntegrity(EndAudioPassThru_response& s)
{
  return checkIntegrityConst(s);
}


bool EndAudioPassThru_responseMarshaller::fromString(const std::string& s,EndAudioPassThru_response& e)
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


const std::string EndAudioPassThru_responseMarshaller::toString(const EndAudioPassThru_response& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool EndAudioPassThru_responseMarshaller::checkIntegrityConst(const EndAudioPassThru_response& s)
{
  if(!ResultMarshaller::checkIntegrityConst(s.resultCode))  return false;
  if(s.info && s.info->length()>1000)  return false;
  return true;
}

Json::Value EndAudioPassThru_responseMarshaller::toJSON(const EndAudioPassThru_response& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["success"]=Json::Value(e.success);

  json["resultCode"]=ResultMarshaller::toJSON(e.resultCode);

  if(e.info)
    json["info"]=Json::Value(*e.info);

  return json;
}


bool EndAudioPassThru_responseMarshaller::fromJSON(const Json::Value& json,EndAudioPassThru_response& c)
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

