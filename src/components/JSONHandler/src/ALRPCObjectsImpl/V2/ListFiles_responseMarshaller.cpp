#include "../include/JSONHandler/ALRPCObjects/V2/ListFiles_response.h"
#include "ResultMarshaller.h"

#include "ListFiles_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


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
  if(!ResultMarshaller::checkIntegrityConst(s.resultCode))  return false;
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

  json["success"]=Json::Value(e.success);

  json["resultCode"]=ResultMarshaller::toJSON(e.resultCode);

  if(e.filenames)
  {
    unsigned int sz=e.filenames->size();
    json["filenames"]=Json::Value(Json::arrayValue);
    json["filenames"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["filenames"][i]=Json::Value(e.filenames[0][i]);
  }

  json["spaceAvailable"]=Json::Value(e.spaceAvailable);

  if(e.info)
    json["info"]=Json::Value(*e.info);

  return json;
}


bool ListFiles_responseMarshaller::fromJSON(const Json::Value& json,ListFiles_response& c)
{
  if(c.filenames)  delete c.filenames;
  c.filenames=0;

  if(c.info)  delete c.info;
  c.info=0;

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

