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

#include "../include/JSONHandler/SDLRPCObjects/V2/OnHMIStatus.h"
#include "AudioStreamingStateMarshaller.h"
#include "HMILevelMarshaller.h"
#include "SystemContextMarshaller.h"

#include "OnHMIStatusMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool OnHMIStatusMarshaller::checkIntegrity(OnHMIStatus& s)
{
  return checkIntegrityConst(s);
}


bool OnHMIStatusMarshaller::fromString(const std::string& s,OnHMIStatus& e)
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


const std::string OnHMIStatusMarshaller::toString(const OnHMIStatus& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnHMIStatusMarshaller::checkIntegrityConst(const OnHMIStatus& s)
{
  if(!HMILevelMarshaller::checkIntegrityConst(s.hmiLevel))  return false;
  if(!AudioStreamingStateMarshaller::checkIntegrityConst(s.audioStreamingState))  return false;
  if(!SystemContextMarshaller::checkIntegrityConst(s.systemContext))  return false;
  return true;
}

Json::Value OnHMIStatusMarshaller::toJSON(const OnHMIStatus& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["hmiLevel"]=HMILevelMarshaller::toJSON(e.hmiLevel);

  json["audioStreamingState"]=AudioStreamingStateMarshaller::toJSON(e.audioStreamingState);

  json["systemContext"]=SystemContextMarshaller::toJSON(e.systemContext);

  return json;
}


bool OnHMIStatusMarshaller::fromJSON(const Json::Value& json,OnHMIStatus& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("hmiLevel"))  return false;
    {
      const Json::Value& j=json["hmiLevel"];
      if(!HMILevelMarshaller::fromJSON(j,c.hmiLevel))
        return false;
    }
    if(!json.isMember("audioStreamingState"))  return false;
    {
      const Json::Value& j=json["audioStreamingState"];
      if(!AudioStreamingStateMarshaller::fromJSON(j,c.audioStreamingState))
        return false;
    }
    if(!json.isMember("systemContext"))  return false;
    {
      const Json::Value& j=json["systemContext"];
      if(!SystemContextMarshaller::fromJSON(j,c.systemContext))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

