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

#include "../include/JSONHandler/SDLRPCObjects/V1/TTSChunk.h"
#include "SpeechCapabilitiesMarshaller.h"

#include "TTSChunkMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPC;


bool TTSChunkMarshaller::checkIntegrity(TTSChunk& s)
{
  return checkIntegrityConst(s);
}


bool TTSChunkMarshaller::fromString(const std::string& s,TTSChunk& e)
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


const std::string TTSChunkMarshaller::toString(const TTSChunk& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool TTSChunkMarshaller::checkIntegrityConst(const TTSChunk& s)
{
  if(s.text.length()>500)  return false;
  if(!SpeechCapabilitiesMarshaller::checkIntegrityConst(s.type))  return false;
  return true;
}

Json::Value TTSChunkMarshaller::toJSON(const TTSChunk& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["text"]=Json::Value(e.text);

  json["type"]=SpeechCapabilitiesMarshaller::toJSON(e.type);


  return json;
}


bool TTSChunkMarshaller::fromJSON(const Json::Value& json,TTSChunk& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("text"))  return false;
    {
      const Json::Value& j=json["text"];
      if(!j.isString())  return false;
      c.text=j.asString();
    }
    if(!json.isMember("type"))  return false;
    {
      const Json::Value& j=json["type"];
      if(!SpeechCapabilitiesMarshaller::fromJSON(j,c.type))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

