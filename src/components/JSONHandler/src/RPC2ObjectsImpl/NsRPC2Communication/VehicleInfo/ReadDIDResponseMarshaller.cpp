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

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/ReadDIDResponse.h"
#include "../src/SDLRPCObjectsImpl/V2/VehicleDataResultCodeMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/ReadDIDResponseMarshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::VehicleInfo;

bool ReadDIDResponseMarshaller::checkIntegrity(ReadDIDResponse& s)
{
  return checkIntegrityConst(s);
}


bool ReadDIDResponseMarshaller::fromString(const std::string& s,ReadDIDResponse& e)
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


const std::string ReadDIDResponseMarshaller::toString(const ReadDIDResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ReadDIDResponseMarshaller::checkIntegrityConst(const ReadDIDResponse& s)
{
  if(s.dataResult)
  {
    unsigned int i=s.dataResult[0].size();
    if(i<0)  return false;
    if(i>1000)  return false;
  }

  if(s.data)
  {
    unsigned int i=s.data[0].size();
    if(i<0)  return false;
    if(i>1000)  return false;
    while(i--)
    {
      if(s.data[0][i].length()>5000)  return false;
    }
  }

  return true;
}


Json::Value ReadDIDResponseMarshaller::toJSON(const ReadDIDResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsSmartDeviceLinkRPCV2::Result r(static_cast<NsSmartDeviceLinkRPCV2::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsSmartDeviceLinkRPCV2::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("VehicleInfo.ReadDIDResponse");

  if(e.dataResult)
  {
    unsigned int i=e.dataResult[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsSmartDeviceLinkRPCV2::VehicleDataResultCodeMarshaller::toJSON(e.dataResult[0][i]);

    json["result"]["dataResult"]=j;
  }
  if(e.data)
  {
    unsigned int i=e.data[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=Json::Value(e.data[0][i]);

    json["result"]["data"]=j;
  }
  return json;
}


bool ReadDIDResponseMarshaller::fromJSON(const Json::Value& json,ReadDIDResponse& c)
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
    if(js["method"].asString().compare("VehicleInfo.ReadDIDResponse")) return false;

    if(!NsSmartDeviceLinkRPCV2::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    if(c.dataResult)  delete c.dataResult;
    c.dataResult=0;
    if(js.isMember("dataResult"))
    {
      if(!js["dataResult"].isArray()) return false;
      unsigned int i=js["dataResult"].size();
      if(i<0)  return false;
      if(i>1000)  return false;

      c.dataResult=new std::vector<NsSmartDeviceLinkRPCV2::VehicleDataResultCode>();
      c.dataResult->resize(js["dataResult"].size());

      while(i--)
        if(!NsSmartDeviceLinkRPCV2::VehicleDataResultCodeMarshaller::fromJSON(js["dataResult"][i],c.dataResult[0][i]))  return false;
    }


    if(c.data)  delete c.data;
    c.data=0;
    if(js.isMember("data"))
    {
      if(!js["data"].isArray()) return false;
      unsigned int i=js["data"].size();
      if(i<0)  return false;
      if(i>1000)  return false;

      c.data=new std::vector<std::string>();
      c.data->resize(js["data"].size());

      while(i--)
      {
        if(!js["data"][i].isString())
          return false;

        c.data[0][i]=js["data"][i].asString();
        if(c.data[0][i].length()>5000)  return false;
      }
    }


  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
