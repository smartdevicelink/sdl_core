#include "../include/JSONHandler/ALRPCObjects/OnEncodedSyncPData_v2.h"


#include "OnEncodedSyncPData_v2Marshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool OnEncodedSyncPData_v2Marshaller::checkIntegrity(OnEncodedSyncPData_v2& s)
{
  return checkIntegrityConst(s);
}


bool OnEncodedSyncPData_v2Marshaller::fromString(const std::string& s,OnEncodedSyncPData_v2& e)
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


const std::string OnEncodedSyncPData_v2Marshaller::toString(const OnEncodedSyncPData_v2& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnEncodedSyncPData_v2Marshaller::checkIntegrityConst(const OnEncodedSyncPData_v2& s)
{
  if(s.data)
  {
    unsigned int i=s.data[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
      if(s.data[0][i].length()>10000)  return false;
    }
  }
  if(s.URL && s.URL->length()>1000)  return false;
  if(s.Timeout && *s.Timeout>2000000000)  return false;
  if(s.Timeout && *s.Timeout<-2000000000)  return false;
  return true;
}

Json::Value OnEncodedSyncPData_v2Marshaller::toJSON(const OnEncodedSyncPData_v2& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["notification"]=Json::Value(Json::objectValue);
  json["notification"]["name"]=Json::Value("OnEncodedSyncPData_v2");

  Json::Value j=Json::Value(Json::objectValue);

  if(e.data)
  {
    unsigned int sz=e.data->size();
    j["data"]=Json::Value(Json::arrayValue);
    j["data"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["data"][i]=Json::Value(e.data[0][i]);
  }

  if(e.URL)
    j["URL"]=Json::Value(*e.URL);

  if(e.Timeout)
    j["Timeout"]=Json::Value(*e.Timeout);

  json["notification"]["parameters"]=j;
  return json;
}


bool OnEncodedSyncPData_v2Marshaller::fromJSON(const Json::Value& js,OnEncodedSyncPData_v2& c)
{
  if(c.data)  delete c.data;
  c.data=0;

  if(c.URL)  delete c.URL;
  c.URL=0;

  if(c.Timeout)  delete c.Timeout;
  c.Timeout=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("notification"))  return false;

    if(!js["notification"].isObject())  return false;
    const Json::Value& j2=js["notification"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("OnEncodedSyncPData_v2"))  return false;

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(json.isMember("data"))
    {
      const Json::Value& j=json["data"];
      if(!j.isArray())  return false;
      c.data=new std::vector<std::string>();
      c.data->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        if(!j[i].isString())
          return false;
        else
          c.data[0][i]=j[i].asString();
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

