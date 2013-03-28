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

#include "../include/JSONHandler/SDLRPCObjects/V2/SetGlobalProperties_request.h"
#include "TTSChunkMarshaller.h"
#include "VrHelpItemMarshaller.h"

#include "SetGlobalProperties_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool SetGlobalProperties_requestMarshaller::checkIntegrity(SetGlobalProperties_request& s)
{
  return checkIntegrityConst(s);
}


bool SetGlobalProperties_requestMarshaller::fromString(const std::string& s,SetGlobalProperties_request& e)
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


const std::string SetGlobalProperties_requestMarshaller::toString(const SetGlobalProperties_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SetGlobalProperties_requestMarshaller::checkIntegrityConst(const SetGlobalProperties_request& s)
{
  if(s.helpPrompt)
  {
    unsigned int i=s.helpPrompt[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TTSChunkMarshaller::checkIntegrityConst(s.helpPrompt[0][i]))   return false;
    }
  }
  if(s.timeoutPrompt)
  {
    unsigned int i=s.timeoutPrompt[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TTSChunkMarshaller::checkIntegrityConst(s.timeoutPrompt[0][i]))   return false;
    }
  }
  if(s.vrHelpTitle && s.vrHelpTitle->length()>500)  return false;
  if(s.vrHelp)
  {
    unsigned int i=s.vrHelp[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!VrHelpItemMarshaller::checkIntegrityConst(s.vrHelp[0][i]))   return false;
    }
  }
  return true;
}

Json::Value SetGlobalProperties_requestMarshaller::toJSON(const SetGlobalProperties_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  if(e.helpPrompt)
  {
    unsigned int sz=e.helpPrompt->size();
    json["helpPrompt"]=Json::Value(Json::arrayValue);
    json["helpPrompt"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["helpPrompt"][i]=TTSChunkMarshaller::toJSON(e.helpPrompt[0][i]);
  }

  if(e.timeoutPrompt)
  {
    unsigned int sz=e.timeoutPrompt->size();
    json["timeoutPrompt"]=Json::Value(Json::arrayValue);
    json["timeoutPrompt"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["timeoutPrompt"][i]=TTSChunkMarshaller::toJSON(e.timeoutPrompt[0][i]);
  }

  if(e.vrHelpTitle)
    json["vrHelpTitle"]=Json::Value(*e.vrHelpTitle);

  if(e.vrHelp)
  {
    unsigned int sz=e.vrHelp->size();
    json["vrHelp"]=Json::Value(Json::arrayValue);
    json["vrHelp"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["vrHelp"][i]=VrHelpItemMarshaller::toJSON(e.vrHelp[0][i]);
  }

  return json;
}


bool SetGlobalProperties_requestMarshaller::fromJSON(const Json::Value& json,SetGlobalProperties_request& c)
{
  if(c.helpPrompt)  delete c.helpPrompt;
  c.helpPrompt=0;

  if(c.timeoutPrompt)  delete c.timeoutPrompt;
  c.timeoutPrompt=0;

  if(c.vrHelpTitle)  delete c.vrHelpTitle;
  c.vrHelpTitle=0;

  if(c.vrHelp)  delete c.vrHelp;
  c.vrHelp=0;

  try
  {
    if(!json.isObject())  return false;

    if(json.isMember("helpPrompt"))
    {
      const Json::Value& j=json["helpPrompt"];
      if(!j.isArray())  return false;
      c.helpPrompt=new std::vector<TTSChunk>();
      c.helpPrompt->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        TTSChunk t;
        if(!TTSChunkMarshaller::fromJSON(j[i],t))
          return false;
        c.helpPrompt[0][i]=t;
      }

    }
    if(json.isMember("timeoutPrompt"))
    {
      const Json::Value& j=json["timeoutPrompt"];
      if(!j.isArray())  return false;
      c.timeoutPrompt=new std::vector<TTSChunk>();
      c.timeoutPrompt->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        TTSChunk t;
        if(!TTSChunkMarshaller::fromJSON(j[i],t))
          return false;
        c.timeoutPrompt[0][i]=t;
      }

    }
    if(json.isMember("vrHelpTitle"))
    {
      const Json::Value& j=json["vrHelpTitle"];
      if(!j.isString())  return false;
      c.vrHelpTitle=new std::string(j.asString());
    }
    if(json.isMember("vrHelp"))
    {
      const Json::Value& j=json["vrHelp"];
      if(!j.isArray())  return false;
      c.vrHelp=new std::vector<VrHelpItem>();
      c.vrHelp->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        VrHelpItem t;
        if(!VrHelpItemMarshaller::fromJSON(j[i],t))
          return false;
        c.vrHelp[0][i]=t;
      }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

