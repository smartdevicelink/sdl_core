#include "../include/JSONHandler/ALRPCObjects/ReadDID_request.h"


#include "ReadDID_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool ReadDID_requestMarshaller::checkIntegrity(ReadDID_request& s)
{
  return checkIntegrityConst(s);
}


bool ReadDID_requestMarshaller::fromString(const std::string& s,ReadDID_request& e)
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


const std::string ReadDID_requestMarshaller::toString(const ReadDID_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ReadDID_requestMarshaller::checkIntegrityConst(const ReadDID_request& s)
{
  if(s.ecuName && *s.ecuName>65535)  return false;
  if(s.didLocation)
  {
    unsigned int i=s.didLocation[0].size();
    if(i>1000 || i<0)  return false;
    while(i--)
    {
      if(s.didLocation[0][i]>65535)  return false;
    }
  }
  return true;
}

Json::Value ReadDID_requestMarshaller::toJSON(const ReadDID_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("ReadDID");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  if(e.ecuName)
    j["ecuName"]=Json::Value(*e.ecuName);

  if(e.didLocation)
  {
    unsigned int sz=e.didLocation->size();
    j["didLocation"]=Json::Value(Json::arrayValue);
    j["didLocation"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["didLocation"][i]=Json::Value(e.didLocation[0][i]);
  }

  if(e.encrypted)
    j["encrypted"]=Json::Value(*e.encrypted);

  json["request"]["parameters"]=j;
  return json;
}


bool ReadDID_requestMarshaller::fromJSON(const Json::Value& js,ReadDID_request& c)
{
  if(c.ecuName)  delete c.ecuName;
  c.ecuName=0;

  if(c.didLocation)  delete c.didLocation;
  c.didLocation=0;

  if(c.encrypted)  delete c.encrypted;
  c.encrypted=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("ReadDID"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(json.isMember("ecuName"))
    {
      const Json::Value& j=json["ecuName"];
      if(!j.isInt())  return false;
      c.ecuName=new unsigned int(j.asInt());
    }
    if(json.isMember("didLocation"))
    {
      const Json::Value& j=json["didLocation"];
      if(!j.isArray())  return false;
      c.didLocation=new std::vector<unsigned int>();
      c.didLocation->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        if(!j[i].isInt())
          return false;
        else
          c.didLocation[0][i]=j[i].asInt();
    }
    if(json.isMember("encrypted"))
    {
      const Json::Value& j=json["encrypted"];
      if(!j.isBool())  return false;
      c.encrypted=new bool(j.asBool());
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

