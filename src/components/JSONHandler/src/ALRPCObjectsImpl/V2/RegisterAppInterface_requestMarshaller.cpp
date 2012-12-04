#include "../include/JSONHandler/ALRPCObjects/V2/RegisterAppInterface_request.h"
#include "AppTypeMarshaller.h"
#include "LanguageMarshaller.h"
#include "SyncMsgVersionMarshaller.h"
#include "TTSChunkMarshaller.h"

#include "RegisterAppInterface_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool RegisterAppInterface_requestMarshaller::checkIntegrity(RegisterAppInterface_request& s)
{
  return checkIntegrityConst(s);
}


bool RegisterAppInterface_requestMarshaller::fromString(const std::string& s,RegisterAppInterface_request& e)
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


const std::string RegisterAppInterface_requestMarshaller::toString(const RegisterAppInterface_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool RegisterAppInterface_requestMarshaller::checkIntegrityConst(const RegisterAppInterface_request& s)
{
  if(!SyncMsgVersionMarshaller::checkIntegrityConst(s.syncMsgVersion))  return false;
  if(s.appName.length()>100)  return false;
  if(s.ttsName)
  {
    unsigned int i=s.ttsName[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TTSChunkMarshaller::checkIntegrityConst(s.ttsName[0][i]))   return false;
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
  if(!LanguageMarshaller::checkIntegrityConst(s.languageDesired))  return false;
  if(!LanguageMarshaller::checkIntegrityConst(s.hmiDisplayLanguageDesired))  return false;
  if(s.appType)
  {
    unsigned int i=s.appType[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!AppTypeMarshaller::checkIntegrityConst(s.appType[0][i]))   return false;
    }
  }
  if(s.appID.length()>100)  return false;
  return true;
}

Json::Value RegisterAppInterface_requestMarshaller::toJSON(const RegisterAppInterface_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["syncMsgVersion"]=SyncMsgVersionMarshaller::toJSON(e.syncMsgVersion);

  json["appName"]=Json::Value(e.appName);

  if(e.ttsName)
  {
    unsigned int sz=e.ttsName->size();
    json["ttsName"]=Json::Value(Json::arrayValue);
    json["ttsName"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["ttsName"][i]=TTSChunkMarshaller::toJSON(e.ttsName[0][i]);
  }

  if(e.ngnMediaScreenAppName)
    json["ngnMediaScreenAppName"]=Json::Value(*e.ngnMediaScreenAppName);

  if(e.vrSynonyms)
  {
    unsigned int sz=e.vrSynonyms->size();
    json["vrSynonyms"]=Json::Value(Json::arrayValue);
    json["vrSynonyms"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["vrSynonyms"][i]=Json::Value(e.vrSynonyms[0][i]);
  }

  json["isMediaApplication"]=Json::Value(e.isMediaApplication);

  json["languageDesired"]=LanguageMarshaller::toJSON(e.languageDesired);

  json["hmiDisplayLanguageDesired"]=LanguageMarshaller::toJSON(e.hmiDisplayLanguageDesired);

  if(e.appType)
  {
    unsigned int sz=e.appType->size();
    json["appType"]=Json::Value(Json::arrayValue);
    json["appType"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["appType"][i]=AppTypeMarshaller::toJSON(e.appType[0][i]);
  }

  json["appID"]=Json::Value(e.appID);

  return json;
}


bool RegisterAppInterface_requestMarshaller::fromJSON(const Json::Value& json,RegisterAppInterface_request& c)
{
  if(c.ttsName)  delete c.ttsName;
  c.ttsName=0;

  if(c.ngnMediaScreenAppName)  delete c.ngnMediaScreenAppName;
  c.ngnMediaScreenAppName=0;

  if(c.vrSynonyms)  delete c.vrSynonyms;
  c.vrSynonyms=0;

  if(c.appType)  delete c.appType;
  c.appType=0;

  try
  {
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
      c.ttsName=new std::vector<TTSChunk>();
      c.ttsName->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        TTSChunk t;
        if(!TTSChunkMarshaller::fromJSON(j[i],t))
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
    if(!json.isMember("languageDesired"))  return false;
    {
      const Json::Value& j=json["languageDesired"];
      if(!LanguageMarshaller::fromJSON(j,c.languageDesired))
        return false;
    }
    if(!json.isMember("hmiDisplayLanguageDesired"))  return false;
    {
      const Json::Value& j=json["hmiDisplayLanguageDesired"];
      if(!LanguageMarshaller::fromJSON(j,c.hmiDisplayLanguageDesired))
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
    if(!json.isMember("appID"))  return false;
    {
      const Json::Value& j=json["appID"];
      if(!j.isString())  return false;
      c.appID=j.asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

