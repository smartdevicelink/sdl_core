#include "../../include/JSONHandler/ALRPCObjects/GetDTCs_response.h"
#include "DTCMarshaller.h"
#include "ResultMarshaller.h"

#include "GetDTCs_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


bool GetDTCs_responseMarshaller::checkIntegrity(GetDTCs_response& s)
{
  return checkIntegrityConst(s);
}


bool GetDTCs_responseMarshaller::fromString(const std::string& s,GetDTCs_response& e)
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


const std::string GetDTCs_responseMarshaller::toString(const GetDTCs_response& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetDTCs_responseMarshaller::checkIntegrityConst(const GetDTCs_response& s)
{
  {
    unsigned int i=s.resultCode.size();
    while(i--)
    {
    if(!ResultMarshaller::checkIntegrityConst(s.resultCode[i]))   return false;
    }
  }
  if(s.info && s.info->length()>1000)  return false;
  if(s.dtcList)
  {
    unsigned int i=s.dtcList[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!DTCMarshaller::checkIntegrityConst(s.dtcList[0][i]))   return false;
    }
  }
  return true;
}

Json::Value GetDTCs_responseMarshaller::toJSON(const GetDTCs_response& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["response"]=Json::Value(Json::objectValue);
  json["response"]["name"]=Json::Value("GetDTCs");
  json["response"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["success"]=Json::Value(e.success);

  j["resultCode"]=Json::Value(Json::arrayValue);
  j["resultCode"].resize(e.resultCode.size());
  for(unsigned int i=0;i<e.resultCode.size();i++)
    j["resultCode"][i]=ResultMarshaller::toJSON(e.resultCode[i]);

  if(e.info)
    j["info"]=Json::Value(*e.info);

  if(e.dtcList)
  {
    unsigned int sz=e.dtcList->size();
    j["dtcList"]=Json::Value(Json::arrayValue);
    j["dtcList"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["dtcList"][i]=DTCMarshaller::toJSON(e.dtcList[0][i]);
  }

  json["response"]["parameters"]=j;
  return json;
}


bool GetDTCs_responseMarshaller::fromJSON(const Json::Value& js,GetDTCs_response& c)
{
  if(c.info)  delete c.info;
  c.info=0;

  if(c.dtcList)  delete c.dtcList;
  c.dtcList=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("response"))  return false;

    if(!js["response"].isObject())  return false;
    const Json::Value& j2=js["response"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("GetDTCs"))  return false;
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
          Result t;
          if(!ResultMarshaller::fromJSON(j[i],t))
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
    if(json.isMember("dtcList"))
    {
      const Json::Value& j=json["dtcList"];
      if(!j.isArray())  return false;
      c.dtcList=new std::vector<DTC>();
      c.dtcList->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        DTC t;
        if(!DTCMarshaller::fromJSON(j[i],t))
          return false;
        c.dtcList[0][i]=t;
      }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

