//
// Copyright (c) 2013 Ford Motor Company
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

