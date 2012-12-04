#include "../include/JSONHandler/ALRPCObjects/V2/OnEncodedSyncPData.h"


#include "OnEncodedSyncPDataMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool OnEncodedSyncPDataMarshaller::checkIntegrity(OnEncodedSyncPData& s)
{
  return checkIntegrityConst(s);
}


bool OnEncodedSyncPDataMarshaller::fromString(const std::string& s,OnEncodedSyncPData& e)
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


const std::string OnEncodedSyncPDataMarshaller::toString(const OnEncodedSyncPData& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnEncodedSyncPDataMarshaller::checkIntegrityConst(const OnEncodedSyncPData& s)
{
  {
    unsigned int i=s.data.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
      if(s.data[i].length()>10000)  return false;
    }
  }
  if(s.URL && s.URL->length()>1000)  return false;
  if(s.Timeout && *s.Timeout>2000000000)  return false;
  if(s.Timeout && *s.Timeout<-2000000000)  return false;
  return true;
}

Json::Value OnEncodedSyncPDataMarshaller::toJSON(const OnEncodedSyncPData& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["data"]=Json::Value(Json::arrayValue);
  json["data"].resize(e.data.size());
  for(unsigned int i=0;i<e.data.size();i++)
    json["data"][i]=Json::Value(e.data[i]);

  if(e.URL)
    json["URL"]=Json::Value(*e.URL);

  if(e.Timeout)
    json["Timeout"]=Json::Value(*e.Timeout);

  return json;
}


bool OnEncodedSyncPDataMarshaller::fromJSON(const Json::Value& json,OnEncodedSyncPData& c)
{
  if(c.URL)  delete c.URL;
  c.URL=0;

  if(c.Timeout)  delete c.Timeout;
  c.Timeout=0;

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
    if(json.isMember("URL"))
    {
      const Json::Value& j=json["URL"];
      if(!j.isString())  return false;
      c.URL=new std::string(j.asString());
    }
    if(json.isMember("Timeout"))
    {
      const Json::Value& j=json["Timeout"];
      if(!j.isInt())  return false;
      c.Timeout=new int(j.asInt());
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

