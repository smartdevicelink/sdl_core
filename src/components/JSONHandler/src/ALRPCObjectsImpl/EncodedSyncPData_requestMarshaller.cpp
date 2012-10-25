#include "../../include/JSONHandler/ALRPCObjects/EncodedSyncPData_request.h"


#include "EncodedSyncPData_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


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

Json::Value EncodedSyncPData_requestMarshaller::toJSON(const EncodedSyncPData_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("EncodedSyncPData");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  if(e.data)
  {
    unsigned int sz=e.data->size();
    j["data"]=Json::Value(Json::arrayValue);
    j["data"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["data"][i]=Json::Value(e.data[0][i]);
  }

  json["request"]["parameters"]=j;
  return json;
}


bool EncodedSyncPData_requestMarshaller::fromJSON(const Json::Value& js,EncodedSyncPData_request& c)
{
  if(c.data)  delete c.data;
  c.data=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("EncodedSyncPData"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

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

