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

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetCapabilitiesResponse.h"
#include "../src/SDLRPCObjectsImpl/V2/DisplayCapabilitiesMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/HmiZoneCapabilitiesMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/SoftButtonCapabilitiesMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/GetCapabilitiesResponseMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;

bool GetCapabilitiesResponseMarshaller::checkIntegrity(GetCapabilitiesResponse& s)
{
  return checkIntegrityConst(s);
}


bool GetCapabilitiesResponseMarshaller::fromString(const std::string& s,GetCapabilitiesResponse& e)
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


const std::string GetCapabilitiesResponseMarshaller::toString(const GetCapabilitiesResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetCapabilitiesResponseMarshaller::checkIntegrityConst(const GetCapabilitiesResponse& s)
{
  if(!NsSmartDeviceLinkRPCV2::DisplayCapabilitiesMarshaller::checkIntegrityConst(s.displayCapabilities))  return false;

  {
    unsigned int i=s.hmiZoneCapabilities.size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  if(s.softButtonCapabilities)
  {
    unsigned int i=s.softButtonCapabilities[0].size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  return true;
}


Json::Value GetCapabilitiesResponseMarshaller::toJSON(const GetCapabilitiesResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsSmartDeviceLinkRPCV2::Result r(static_cast<NsSmartDeviceLinkRPCV2::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsSmartDeviceLinkRPCV2::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("UI.GetCapabilitiesResponse");

  json["result"]["displayCapabilities"]=NsSmartDeviceLinkRPCV2::DisplayCapabilitiesMarshaller::toJSON(e.displayCapabilities);;
  {
    unsigned int i=e.hmiZoneCapabilities.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsSmartDeviceLinkRPCV2::HmiZoneCapabilitiesMarshaller::toJSON(e.hmiZoneCapabilities[i]);

    json["result"]["hmiZoneCapabilities"]=j;
  }
  if(e.softButtonCapabilities)
  {
    unsigned int i=e.softButtonCapabilities[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsSmartDeviceLinkRPCV2::SoftButtonCapabilitiesMarshaller::toJSON(e.softButtonCapabilities[0][i]);

    json["result"]["softButtonCapabilities"]=j;
  }
  return json;
}


bool GetCapabilitiesResponseMarshaller::fromJSON(const Json::Value& json,GetCapabilitiesResponse& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("result")) return false;

    Json::Value js=json["result"];
    if(!js.isObject())  return false;

    NsSmartDeviceLinkRPCV2::Result r;
    if(!js.isMember("resultCode") || !js["resultCode"].isString())  return false;
    if(!js.isMember("method") || !js["method"].isString())  return false;
    if(js["method"].asString().compare("UI.GetCapabilitiesResponse")) return false;

    if(!NsSmartDeviceLinkRPCV2::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    if(!js.isMember("displayCapabilities") || !NsSmartDeviceLinkRPCV2::DisplayCapabilitiesMarshaller::fromJSON(js["displayCapabilities"],c.displayCapabilities))  return false;

    if(!js.isMember("hmiZoneCapabilities") || !js["hmiZoneCapabilities"].isArray())  return false;
    {
      unsigned int i=js["hmiZoneCapabilities"].size();
      if(i<1)  return false;
      if(i>100)  return false;
      c.hmiZoneCapabilities.resize(i);
      while(i--)
      {
        NsSmartDeviceLinkRPCV2::HmiZoneCapabilities t;
        if(!NsSmartDeviceLinkRPCV2::HmiZoneCapabilitiesMarshaller::fromJSON(js["hmiZoneCapabilities"][i],t))
          return false;
         c.hmiZoneCapabilities[i]=t;
      }
    }

    if(c.softButtonCapabilities)  delete c.softButtonCapabilities;
    c.softButtonCapabilities=0;
    if(js.isMember("softButtonCapabilities"))
    {
      if(!js["softButtonCapabilities"].isArray()) return false;
      unsigned int i=js["softButtonCapabilities"].size();
      if(i<1)  return false;
      if(i>100)  return false;

      c.softButtonCapabilities=new std::vector<NsSmartDeviceLinkRPCV2::SoftButtonCapabilities>();
      c.softButtonCapabilities->resize(js["softButtonCapabilities"].size());

      while(i--)
        if(!NsSmartDeviceLinkRPCV2::SoftButtonCapabilitiesMarshaller::fromJSON(js["softButtonCapabilities"][i],c.softButtonCapabilities[0][i]))  return false;
    }


  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
