#include "../include/JSONHandler/SDLRPCObjects/V2/EncodedSyncPData_request.h"


#include "EncodedSyncPData_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsSmartDeviceLinkRPCV2;


bool EncodedSyncPData_requestMarshaller::checkIntegrity(EncodedSyncPData_request& s)
{
  return checkIntegrityConst(s);
}


bool EncodedSyncPData_requestMarshaller::fromString(const std::string& s,EncodedSyncPData_request& e)
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


const std::string EncodedSyncPData_requestMarshaller::toString(const EncodedSyncPData_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool EncodedSyncPData_requestMarshaller::checkIntegrityConst(const EncodedSyncPData_request& s)
{
  {
    unsigned int i=s.data.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
      if(s.data[i].length()>10000)  return false;
    }
  }
  return true;
}

Json::Value EncodedSyncPData_requestMarshaller::toJSON(const EncodedSyncPData_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["data"]=Json::Value(Json::arrayValue);
  json["data"].resize(e.data.size());
  for(unsigned int i=0;i<e.data.size();i++)
    json["data"][i]=Json::Value(e.data[i]);

  return json;
}


bool EncodedSyncPData_requestMarshaller::fromJSON(const Json::Value& json,EncodedSyncPData_request& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("data"))  return false;
    {
      const Json::Value& j=json["data"];
      if(!j.isArray())  return false;
      c.data.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        if(!j[i].isString())
          return false;
        else
          c.data[i]=j[i].asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

