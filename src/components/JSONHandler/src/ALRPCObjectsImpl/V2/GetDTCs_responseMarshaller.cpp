#include "../include/JSONHandler/ALRPCObjects/V2/GetDTCs_response.h"
#include "DTCMarshaller.h"
#include "ResultMarshaller.h"

#include "GetDTCs_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


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
  if(!ResultMarshaller::checkIntegrityConst(s.resultCode))  return false;
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

  json["success"]=Json::Value(e.success);

  json["resultCode"]=ResultMarshaller::toJSON(e.resultCode);

  if(e.info)
    json["info"]=Json::Value(*e.info);

  if(e.dtcList)
  {
    unsigned int sz=e.dtcList->size();
    json["dtcList"]=Json::Value(Json::arrayValue);
    json["dtcList"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["dtcList"][i]=DTCMarshaller::toJSON(e.dtcList[0][i]);
  }

  return json;
}


bool GetDTCs_responseMarshaller::fromJSON(const Json::Value& json,GetDTCs_response& c)
{
  if(c.info)  delete c.info;
  c.info=0;

  if(c.dtcList)  delete c.dtcList;
  c.dtcList=0;

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

