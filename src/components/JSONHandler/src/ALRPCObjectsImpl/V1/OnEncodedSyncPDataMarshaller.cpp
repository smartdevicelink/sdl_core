#include "../include/JSONHandler/ALRPCObjects/V1/OnEncodedSyncPData.h"


#include "OnEncodedSyncPDataMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV1;


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
  if(s.data)
  {
    unsigned int i=s.data[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
      if(s.data[0][i].length()>10000)  return false;
    }
  }
  return true;
}

Json::Value OnEncodedSyncPDataMarshaller::toJSON(const OnEncodedSyncPData& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["notification"]=Json::Value(Json::objectValue);
  json["notification"]["name"]=Json::Value("OnEncodedSyncPData");

  Json::Value j=Json::Value(Json::objectValue);

  if(e.data)
  {
    unsigned int sz=e.data->size();
    j["data"]=Json::Value(Json::arrayValue);
    j["data"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["data"][i]=Json::Value(e.data[0][i]);
  }

  json["notification"]["parameters"]=j;
  return json;
}


bool OnEncodedSyncPDataMarshaller::fromJSON(const Json::Value& js,OnEncodedSyncPData& c)
{
  if(c.data)  delete c.data;
  c.data=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("notification"))  return false;

    if(!js["notification"].isObject())  return false;
    const Json::Value& j2=js["notification"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("OnEncodedSyncPData"))  return false;

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

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

