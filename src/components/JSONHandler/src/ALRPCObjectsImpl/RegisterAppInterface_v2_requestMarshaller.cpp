#include "../include/JSONHandler/ALRPCObjects/RegisterAppInterface_v2_request.h"
#include "AppTypeMarshaller.h"
#include "Language_v2Marshaller.h"
#include "SyncMsgVersionMarshaller.h"
#include "TTSChunk_v2Marshaller.h"

#include "RegisterAppInterface_v2_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool RegisterAppInterface_v2_requestMarshaller::checkIntegrity(RegisterAppInterface_v2_request& s)
{
  return checkIntegrityConst(s);
}


bool RegisterAppInterface_v2_requestMarshaller::fromString(const std::string& s,RegisterAppInterface_v2_request& e)
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


const std::string RegisterAppInterface_v2_requestMarshaller::toString(const RegisterAppInterface_v2_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool RegisterAppInterface_v2_requestMarshaller::checkIntegrityConst(const RegisterAppInterface_v2_request& s)
{
  if(!SyncMsgVersionMarshaller::checkIntegrityConst(s.syncMsgVersion))  return false;
  if(s.appName.length()>100)  return false;
  if(s.ttsName)
  {
    unsigned int i=s.ttsName[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TTSChunk_v2Marshaller::checkIntegrityConst(s.ttsName[0][i]))   return false;
    }
  }
  if(s.ngnMediaScreenAppName && s.ngnMediaScreenAppName->length()>100)  return false;
  if(s.vrSynonyms)
  {
    unsigned int i=s.vrSynonyms[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
      if(s.vrSynonyms[0][i].length()>40)  return false;
    }
  }
  if(s.languageDesired && !Language_v2Marshaller::checkIntegrityConst(*s.languageDesired))  return false;
  if(s.hmiDisplayLanguageDesired && !Language_v2Marshaller::checkIntegrityConst(*s.hmiDisplayLanguageDesired))  return false;
  if(s.appType)
  {
    unsigned int i=s.appType[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!AppTypeMarshaller::checkIntegrityConst(s.appType[0][i]))   return false;
    }
  }
  if(s.appID && s.appID->length()>100)  return false;
  return true;
}

Json::Value RegisterAppInterface_v2_requestMarshaller::toJSON(const RegisterAppInterface_v2_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("RegisterAppInterface_v2");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["syncMsgVersion"]=SyncMsgVersionMarshaller::toJSON(e.syncMsgVersion);

  j["appName"]=Json::Value(e.appName);

  if(e.ttsName)
  {
    unsigned int sz=e.ttsName->size();
    j["ttsName"]=Json::Value(Json::arrayValue);
    j["ttsName"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["ttsName"][i]=TTSChunk_v2Marshaller::toJSON(e.ttsName[0][i]);
  }

  if(e.ngnMediaScreenAppName)
    j["ngnMediaScreenAppName"]=Json::Value(*e.ngnMediaScreenAppName);

  if(e.vrSynonyms)
  {
    unsigned int sz=e.vrSynonyms->size();
    j["vrSynonyms"]=Json::Value(Json::arrayValue);
    j["vrSynonyms"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["vrSynonyms"][i]=Json::Value(e.vrSynonyms[0][i]);
  }

  j["isMediaApplication"]=Json::Value(e.isMediaApplication);

  if(e.languageDesired)
    j["languageDesired"]=Language_v2Marshaller::toJSON(*e.languageDesired);

  if(e.hmiDisplayLanguageDesired)
    j["hmiDisplayLanguageDesired"]=Language_v2Marshaller::toJSON(*e.hmiDisplayLanguageDesired);

  if(e.appType)
  {
    unsigned int sz=e.appType->size();
    j["appType"]=Json::Value(Json::arrayValue);
    j["appType"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["appType"][i]=AppTypeMarshaller::toJSON(e.appType[0][i]);
  }

  if(e.appID)
    j["appID"]=Json::Value(*e.appID);

  json["request"]["parameters"]=j;
  return json;
}


bool RegisterAppInterface_v2_requestMarshaller::fromJSON(const Json::Value& js,RegisterAppInterface_v2_request& c)
{
  if(c.ttsName)  delete c.ttsName;
  c.ttsName=0;

  if(c.ngnMediaScreenAppName)  delete c.ngnMediaScreenAppName;
  c.ngnMediaScreenAppName=0;

  if(c.vrSynonyms)  delete c.vrSynonyms;
  c.vrSynonyms=0;

  if(c.languageDesired)  delete c.languageDesired;
  c.languageDesired=0;

  if(c.hmiDisplayLanguageDesired)  delete c.hmiDisplayLanguageDesired;
  c.hmiDisplayLanguageDesired=0;

  if(c.appType)  delete c.appType;
  c.appType=0;

  if(c.appID)  delete c.appID;
  c.appID=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("RegisterAppInterface_v2"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("syncMsgVersion"))  return false;
    {
      const Json::Value& j=json["syncMsgVersion"];
      if(!SyncMsgVersionMarshaller::fromJSON(j,c.syncMsgVersion))
        return false;
    }
    if(!json.isMember("appName"))  return false;
    {
      const Json::Value& j=json["appName"];
      if(!j.isString())  return false;
      c.appName=j.asString();
    }
    if(json.isMember("ttsName"))
    {
      const Json::Value& j=json["ttsName"];
      if(!j.isArray())  return false;
      c.ttsName=new std::vector<TTSChunk_v2>();
      c.ttsName->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        TTSChunk_v2 t;
        if(!TTSChunk_v2Marshaller::fromJSON(j[i],t))
          return false;
        c.ttsName[0][i]=t;
      }

    }
    if(json.isMember("ngnMediaScreenAppName"))
    {
      const Json::Value& j=json["ngnMediaScreenAppName"];
      if(!j.isString())  return false;
      c.ngnMediaScreenAppName=new std::string(j.asString());
    }
    if(json.isMember("vrSynonyms"))
    {
      const Json::Value& j=json["vrSynonyms"];
      if(!j.isArray())  return false;
      c.vrSynonyms=new std::vector<std::string>();
      c.vrSynonyms->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        if(!j[i].isString())
          return false;
        else
          c.vrSynonyms[0][i]=j[i].asString();
    }
    if(!json.isMember("isMediaApplication"))  return false;
    {
      const Json::Value& j=json["isMediaApplication"];
      if(!j.isBool())  return false;
      c.isMediaApplication=j.asBool();
    }
    if(json.isMember("languageDesired"))
    {
      const Json::Value& j=json["languageDesired"];
      c.languageDesired=new Language_v2();
      if(!Language_v2Marshaller::fromJSON(j,c.languageDesired[0]))
        return false;
    }
    if(json.isMember("hmiDisplayLanguageDesired"))
    {
      const Json::Value& j=json["hmiDisplayLanguageDesired"];
      c.hmiDisplayLanguageDesired=new Language_v2();
      if(!Language_v2Marshaller::fromJSON(j,c.hmiDisplayLanguageDesired[0]))
        return false;
    }
    if(json.isMember("appType"))
    {
      const Json::Value& j=json["appType"];
      if(!j.isArray())  return false;
      c.appType=new std::vector<AppType>();
      c.appType->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        AppType t;
        if(!AppTypeMarshaller::fromJSON(j[i],t))
          return false;
        c.appType[0][i]=t;
      }

    }
    if(json.isMember("appID"))
    {
      const Json::Value& j=json["appID"];
      if(!j.isString())  return false;
      c.appID=new std::string(j.asString());
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

