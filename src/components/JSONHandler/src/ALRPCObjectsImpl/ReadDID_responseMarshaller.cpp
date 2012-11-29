#include "../include/JSONHandler/ALRPCObjects/ReadDID_response.h"
#include "Result_v2Marshaller.h"
#include "VehicleDataResultCodeMarshaller.h"

#include "ReadDID_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool ReadDID_responseMarshaller::checkIntegrity(ReadDID_response& s)
{
  return checkIntegrityConst(s);
}


bool ReadDID_responseMarshaller::fromString(const std::string& s,ReadDID_response& e)
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


const std::string ReadDID_responseMarshaller::toString(const ReadDID_response& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ReadDID_responseMarshaller::checkIntegrityConst(const ReadDID_response& s)
{
  {
    unsigned int i=s.resultCode.size();
    while(i--)
    {
    if(!Result_v2Marshaller::checkIntegrityConst(s.resultCode[i]))   return false;
    }
  }
  if(s.info && s.info->length()>1000)  return false;
  if(s.dataResult)
  {
    unsigned int i=s.dataResult[0].size();
    if(i>1000 || i<0)  return false;
    while(i--)
    {
    if(!VehicleDataResultCodeMarshaller::checkIntegrityConst(s.dataResult[0][i]))   return false;
    }
  }
  if(s.data)
  {
    unsigned int i=s.data[0].size();
    if(i>1000 || i<0)  return false;
    while(i--)
    {
      if(s.data[0][i].length()>5000)  return false;
    }
  }
  return true;
}

Json::Value ReadDID_responseMarshaller::toJSON(const ReadDID_response& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["response"]=Json::Value(Json::objectValue);
  json["response"]["name"]=Json::Value("ReadDID");
  json["response"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["success"]=Json::Value(e.success);

  j["resultCode"]=Json::Value(Json::arrayValue);
  j["resultCode"].resize(e.resultCode.size());
  for(unsigned int i=0;i<e.resultCode.size();i++)
    j["resultCode"][i]=Result_v2Marshaller::toJSON(e.resultCode[i]);

  if(e.info)
    j["info"]=Json::Value(*e.info);

  if(e.dataResult)
  {
    unsigned int sz=e.dataResult->size();
    j["dataResult"]=Json::Value(Json::arrayValue);
    j["dataResult"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["dataResult"][i]=VehicleDataResultCodeMarshaller::toJSON(e.dataResult[0][i]);
  }

  if(e.data)
  {
    unsigned int sz=e.data->size();
    j["data"]=Json::Value(Json::arrayValue);
    j["data"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["data"][i]=Json::Value(e.data[0][i]);
  }

  json["response"]["parameters"]=j;
  return json;
}


bool ReadDID_responseMarshaller::fromJSON(const Json::Value& js,ReadDID_response& c)
{
  if(c.info)  delete c.info;
  c.info=0;

  if(c.dataResult)  delete c.dataResult;
  c.dataResult=0;

  if(c.data)  delete c.data;
  c.data=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("response"))  return false;

    if(!js["response"].isObject())  return false;
    const Json::Value& j2=js["response"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("ReadDID"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
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
      if(!j.isArray())  return false;
      c.resultCode.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          Result_v2 t;
          if(!Result_v2Marshaller::fromJSON(j[i],t))
            return false;
          c.resultCode[i]=t;
        }

    }
    if(json.isMember("info"))
    {
      const Json::Value& j=json["info"];
      if(!j.isString())  return false;
      c.info=new std::string(j.asString());
    }
    if(json.isMember("dataResult"))
    {
      const Json::Value& j=json["dataResult"];
      if(!j.isArray())  return false;
      c.dataResult=new std::vector<VehicleDataResultCode>();
      c.dataResult->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        VehicleDataResultCode t;
        if(!VehicleDataResultCodeMarshaller::fromJSON(j[i],t))
          return false;
        c.dataResult[0][i]=t;
      }

    }
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

