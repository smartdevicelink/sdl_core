//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/PutFile_request.h"
#include "FileTypeMarshaller.h"

#include "PutFile_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


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
  if(s.syncFileName.length()>500)  return false;
  if(!FileTypeMarshaller::checkIntegrityConst(s.fileType))  return false;
  return true;
}

Json::Value PutFile_requestMarshaller::toJSON(const PutFile_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["syncFileName"]=Json::Value(e.syncFileName);

  json["fileType"]=FileTypeMarshaller::toJSON(e.fileType);

  if(e.persistentFile)
    json["persistentFile"]=Json::Value(*e.persistentFile);

  if(e.fileData)
    json["fileData"]=Json::Value(*e.fileData);

  return json;
}


bool PutFile_requestMarshaller::fromJSON(const Json::Value& json,PutFile_request& c)
{
  if(c.persistentFile)  delete c.persistentFile;
  c.persistentFile=0;

  if(c.fileData)  delete c.fileData;
  c.fileData=0;

  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("syncFileName"))  return false;
    {
      const Json::Value& j=json["syncFileName"];
      if(!j.isString())  return false;
      c.syncFileName=j.asString();
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
    if(json.isMember("fileData"))
    {
      const Json::Value& j=json["fileData"];
      if(!j.isString())  return false;
      c.fileData=new std::string(j.asString());
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

