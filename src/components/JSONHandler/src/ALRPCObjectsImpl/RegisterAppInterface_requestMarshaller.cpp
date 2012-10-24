#include "../../include/JSONHandler/ALRPCObjects/RegisterAppInterface_request.h"
#include "LanguageMarshaller.h"
#include "SyncMsgVersionMarshaller.h"

#include "RegisterAppInterface_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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
  if(s.languageDesired && !LanguageMarshaller::checkIntegrityConst(*s.languageDesired))  return false;
  if(s.hmiDisplayLanguageDesired && !LanguageMarshaller::checkIntegrityConst(*s.hmiDisplayLanguageDesired))  return false;
  if(s.autoActivateID && s.autoActivateID->length()>16)  return false;
  if(s.appID && s.appID->length()>100)  return false;
  return true;
}

Json::Value RegisterAppInterface_requestMarshaller::toJSON(const RegisterAppInterface_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("RegisterAppInterface");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["syncMsgVersion"]=SyncMsgVersionMarshaller::toJSON(e.syncMsgVersion);

  j["appName"]=Json::Value(e.appName);

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

  if(e.usesVehicleData)
    j["usesVehicleData"]=Json::Value(*e.usesVehicleData);

  j["isMediaApplication"]=Json::Value(e.isMediaApplication);

  if(e.languageDesired)
    j["languageDesired"]=LanguageMarshaller::toJSON(*e.languageDesired);

  if(e.hmiDisplayLanguageDesired)
    j["hmiDisplayLanguageDesired"]=LanguageMarshaller::toJSON(*e.hmiDisplayLanguageDesired);

  if(e.autoActivateID)
    j["autoActivateID"]=Json::Value(*e.autoActivateID);

  if(e.appID)
    j["appID"]=Json::Value(*e.appID);

  json["request"]["parameters"]=j;
  return json;
}


bool RegisterAppInterface_requestMarshaller::fromJSON(const Json::Value& js,RegisterAppInterface_request& c)
{
  if(c.ngnMediaScreenAppName)  delete c.ngnMediaScreenAppName;
  c.ngnMediaScreenAppName=0;

  if(c.vrSynonyms)  delete c.vrSynonyms;
  c.vrSynonyms=0;

  if(c.usesVehicleData)  delete c.usesVehicleData;
  c.usesVehicleData=0;

  if(c.languageDesired)  delete c.languageDesired;
  c.languageDesired=0;

  if(c.hmiDisplayLanguageDesired)  delete c.hmiDisplayLanguageDesired;
  c.hmiDisplayLanguageDesired=0;

  if(c.autoActivateID)  delete c.autoActivateID;
  c.autoActivateID=0;

  if(c.appID)  delete c.appID;
  c.appID=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("RegisterAppInterface"))  return false;
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
    if(json.isMember("usesVehicleData"))
    {
      const Json::Value& j=json["usesVehicleData"];
      if(!j.isBool())  return false;
      c.usesVehicleData=new bool(j.asBool());
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
      c.languageDesired=new Language();
      if(!LanguageMarshaller::fromJSON(j,c.languageDesired[0]))
        return false;
    }
    if(json.isMember("hmiDisplayLanguageDesired"))
    {
      const Json::Value& j=json["hmiDisplayLanguageDesired"];
      c.hmiDisplayLanguageDesired=new Language();
      if(!LanguageMarshaller::fromJSON(j,c.hmiDisplayLanguageDesired[0]))
        return false;
    }
    if(json.isMember("autoActivateID"))
    {
      const Json::Value& j=json["autoActivateID"];
      if(!j.isString())  return false;
      c.autoActivateID=new std::string(j.asString());
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

