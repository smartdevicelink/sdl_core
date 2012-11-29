#include "../include/JSONHandler/ALRPCObjects/ListFiles_response.h"
#include "Result_v2Marshaller.h"

#include "ListFiles_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool ListFiles_responseMarshaller::checkIntegrity(ListFiles_response& s)
{
  return checkIntegrityConst(s);
}


bool ListFiles_responseMarshaller::fromString(const std::string& s,ListFiles_response& e)
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


const std::string ListFiles_responseMarshaller::toString(const ListFiles_response& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ListFiles_responseMarshaller::checkIntegrityConst(const ListFiles_response& s)
{
  if(!Result_v2Marshaller::checkIntegrityConst(s.resultCode))  return false;
  if(s.filenames)
  {
    unsigned int i=s.filenames[0].size();
    if(i>1000 || i<0)  return false;
    while(i--)
    {
      if(s.filenames[0][i].length()>500)  return false;
    }
  }
  if(s.spaceAvailable>2000000000)  return false;
  if(s.info && s.info->length()>1000)  return false;
  return true;
}

Json::Value ListFiles_responseMarshaller::toJSON(const ListFiles_response& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["response"]=Json::Value(Json::objectValue);
  json["response"]["name"]=Json::Value("ListFiles");
  json["response"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["success"]=Json::Value(e.success);

  j["resultCode"]=Result_v2Marshaller::toJSON(e.resultCode);

  if(e.filenames)
  {
    unsigned int sz=e.filenames->size();
    j["filenames"]=Json::Value(Json::arrayValue);
    j["filenames"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["filenames"][i]=Json::Value(e.filenames[0][i]);
  }

  j["spaceAvailable"]=Json::Value(e.spaceAvailable);

  if(e.info)
    j["info"]=Json::Value(*e.info);

  json["response"]["parameters"]=j;
  return json;
}


bool ListFiles_responseMarshaller::fromJSON(const Json::Value& js,ListFiles_response& c)
{
  if(c.filenames)  delete c.filenames;
  c.filenames=0;

  if(c.info)  delete c.info;
  c.info=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("response"))  return false;

    if(!js["response"].isObject())  return false;
    const Json::Value& j2=js["response"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("ListFiles"))  return false;
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
      if(!Result_v2Marshaller::fromJSON(j,c.resultCode))
        return false;
    }
    if(json.isMember("filenames"))
    {
      const Json::Value& j=json["filenames"];
      if(!j.isArray())  return false;
      c.filenames=new std::vector<std::string>();
      c.filenames->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        if(!j[i].isString())
          return false;
        else
          c.filenames[0][i]=j[i].asString();
    }
    if(!json.isMember("spaceAvailable"))  return false;
    {
      const Json::Value& j=json["spaceAvailable"];
      if(!j.isInt())  return false;
      c.spaceAvailable=j.asInt();
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

