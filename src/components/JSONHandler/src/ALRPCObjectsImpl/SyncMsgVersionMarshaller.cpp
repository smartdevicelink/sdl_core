#include "../../include/JSONHandler/ALRPCObjects/SyncMsgVersion.h"


#include "SyncMsgVersionMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


bool SyncMsgVersionMarshaller::checkIntegrity(SyncMsgVersion& s)
{
  return checkIntegrityConst(s);
}


bool SyncMsgVersionMarshaller::fromString(const std::string& s,SyncMsgVersion& e)
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


const std::string SyncMsgVersionMarshaller::toString(const SyncMsgVersion& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SyncMsgVersionMarshaller::checkIntegrityConst(const SyncMsgVersion& s)
{
  if(s.majorVersion>1)  return false;
  if(s.majorVersion<1)  return false;
  if(s.minorVersion>1000)  return false;
  return true;
}

Json::Value SyncMsgVersionMarshaller::toJSON(const SyncMsgVersion& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["majorVersion"]=Json::Value(e.majorVersion);

  json["minorVersion"]=Json::Value(e.minorVersion);


  return json;
}


bool SyncMsgVersionMarshaller::fromJSON(const Json::Value& json,SyncMsgVersion& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("majorVersion"))  return false;
    {
      const Json::Value& j=json["majorVersion"];
      if(!j.isInt())  return false;
      c.majorVersion=j.asInt();
    }
    if(!json.isMember("minorVersion"))  return false;
    {
      const Json::Value& j=json["minorVersion"];
      if(!j.isInt())  return false;
      c.minorVersion=j.asInt();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

