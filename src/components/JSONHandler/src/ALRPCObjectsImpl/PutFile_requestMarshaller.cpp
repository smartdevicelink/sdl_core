#include "../include/JSONHandler/ALRPCObjects/PutFile_request.h"
#include "FileTypeMarshaller.h"

#include "PutFile_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool PutFile_requestMarshaller::checkIntegrity(PutFile_request& s)
{
  return checkIntegrityConst(s);
}


bool PutFile_requestMarshaller::fromString(const std::string& s,PutFile_request& e)
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


const std::string PutFile_requestMarshaller::toString(const PutFile_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool PutFile_requestMarshaller::checkIntegrityConst(const PutFile_request& s)
{
  if(s.syncFileName && s.syncFileName->length()>500)  return false;
  if(!FileTypeMarshaller::checkIntegrityConst(s.fileType))  return false;
  return true;
}

Json::Value PutFile_requestMarshaller::toJSON(const PutFile_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("PutFile");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  if(e.syncFileName)
    j["syncFileName"]=Json::Value(*e.syncFileName);

  j["fileType"]=FileTypeMarshaller::toJSON(e.fileType);

  if(e.persistentFile)
    j["persistentFile"]=Json::Value(*e.persistentFile);

  j["fileData"]=Json::Value(e.fileData);

  json["request"]["parameters"]=j;
  return json;
}


bool PutFile_requestMarshaller::fromJSON(const Json::Value& js,PutFile_request& c)
{
  if(c.syncFileName)  delete c.syncFileName;
  c.syncFileName=0;

  if(c.persistentFile)  delete c.persistentFile;
  c.persistentFile=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("PutFile"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(json.isMember("syncFileName"))
    {
      const Json::Value& j=json["syncFileName"];
      if(!j.isString())  return false;
      c.syncFileName=new std::string(j.asString());
    }
    if(!json.isMember("fileType"))  return false;
    {
      const Json::Value& j=json["fileType"];
      if(!FileTypeMarshaller::fromJSON(j,c.fileType))
        return false;
    }
    if(json.isMember("persistentFile"))
    {
      const Json::Value& j=json["persistentFile"];
      if(!j.isBool())  return false;
      c.persistentFile=new bool(j.asBool());
    }
    if(!json.isMember("fileData"))  return false;
    {
      const Json::Value& j=json["fileData"];
      if(!j.isString())  return false;
      c.fileData=j.asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

