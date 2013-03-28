//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "../include/JSONHandler/SDLRPCObjects/V2/HMIApplication.h"
#include "AppTypeMarshaller.h"
#include "LanguageMarshaller.h"
#include "TTSChunkMarshaller.h"

#include "HMIApplicationMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool HMIApplicationMarshaller::checkIntegrity(HMIApplication& s)
{
  return checkIntegrityConst(s);
}


bool HMIApplicationMarshaller::fromString(const std::string& s,HMIApplication& e)
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


const std::string HMIApplicationMarshaller::toString(const HMIApplication& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool HMIApplicationMarshaller::checkIntegrityConst(const HMIApplication& s)
{
  if(s.appName.length()>100)  return false;
  if(s.appType)
  {
    unsigned int i=s.appType[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!AppTypeMarshaller::checkIntegrityConst(s.appType[0][i]))   return false;
    }
  }
  if(!LanguageMarshaller::checkIntegrityConst(s.hmiDisplayLanguageDesired))  return false;
  if(!LanguageMarshaller::checkIntegrityConst(s.languageDesired))  return false;
  if(s.ngnMediaScreenAppName && s.ngnMediaScreenAppName->length()>100)  return false;
  if(s.ttsName)
  {
    unsigned int i=s.ttsName[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TTSChunkMarshaller::checkIntegrityConst(s.ttsName[0][i]))   return false;
    }
  }
  if(s.vrSynonyms)
  {
    unsigned int i=s.vrSynonyms[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
      if(s.vrSynonyms[0][i].length()>40)  return false;
    }
  }
  return true;
}

Json::Value HMIApplicationMarshaller::toJSON(const HMIApplication& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["appId"]=Json::Value(e.appId);

  json["appName"]=Json::Value(e.appName);

  if(e.appType)
  {
    unsigned int sz=e.appType->size();
    json["appType"]=Json::Value(Json::arrayValue);
    json["appType"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["appType"][i]=AppTypeMarshaller::toJSON(e.appType[0][i]);
  }

  json["deviceName"]=Json::Value(e.deviceName);

  json["hmiDisplayLanguageDesired"]=LanguageMarshaller::toJSON(e.hmiDisplayLanguageDesired);

  if(e.icon)
    json["icon"]=Json::Value(*e.icon);

  json["isMediaApplication"]=Json::Value(e.isMediaApplication);

  json["languageDesired"]=LanguageMarshaller::toJSON(e.languageDesired);

  if(e.ngnMediaScreenAppName)
    json["ngnMediaScreenAppName"]=Json::Value(*e.ngnMediaScreenAppName);

  if(e.ttsName)
  {
    unsigned int sz=e.ttsName->size();
    json["ttsName"]=Json::Value(Json::arrayValue);
    json["ttsName"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["ttsName"][i]=TTSChunkMarshaller::toJSON(e.ttsName[0][i]);
  }

  if(e.vrSynonyms)
  {
    unsigned int sz=e.vrSynonyms->size();
    json["vrSynonyms"]=Json::Value(Json::arrayValue);
    json["vrSynonyms"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["vrSynonyms"][i]=Json::Value(e.vrSynonyms[0][i]);
  }


  return json;
}


bool HMIApplicationMarshaller::fromJSON(const Json::Value& json,HMIApplication& c)
{
  if(c.appType)  delete c.appType;
  c.appType=0;

  if(c.icon)  delete c.icon;
  c.icon=0;

  if(c.ngnMediaScreenAppName)  delete c.ngnMediaScreenAppName;
  c.ngnMediaScreenAppName=0;

  if(c.ttsName)  delete c.ttsName;
  c.ttsName=0;

  if(c.vrSynonyms)  delete c.vrSynonyms;
  c.vrSynonyms=0;

  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("appId"))  return false;
    {
      const Json::Value& j=json["appId"];
      if(!j.isInt())  return false;
      c.appId=j.asInt();
    }
    if(!json.isMember("appName"))  return false;
    {
      const Json::Value& j=json["appName"];
      if(!j.isString())  return false;
      c.appName=j.asString();
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
    if(!json.isMember("deviceName"))  return false;
    {
      const Json::Value& j=json["deviceName"];
      if(!j.isString())  return false;
      c.deviceName=j.asString();
    }
    if(!json.isMember("hmiDisplayLanguageDesired"))  return false;
    {
      const Json::Value& j=json["hmiDisplayLanguageDesired"];
      if(!LanguageMarshaller::fromJSON(j,c.hmiDisplayLanguageDesired))
        return false;
    }
    if(json.isMember("icon"))
    {
      const Json::Value& j=json["icon"];
      if(!j.isString())  return false;
      c.icon=new std::string(j.asString());
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
    if(json.isMember("ngnMediaScreenAppName"))
    {
      const Json::Value& j=json["ngnMediaScreenAppName"];
      if(!j.isString())  return false;
      c.ngnMediaScreenAppName=new std::string(j.asString());
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

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

