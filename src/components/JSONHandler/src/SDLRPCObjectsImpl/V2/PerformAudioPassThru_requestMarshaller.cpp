#include "../include/JSONHandler/SDLRPCObjects/V2/PerformAudioPassThru_request.h"
#include "AudioCaptureQualityMarshaller.h"
#include "AudioTypeMarshaller.h"
#include "SamplingRateMarshaller.h"
#include "TTSChunkMarshaller.h"

#include "PerformAudioPassThru_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsSmartDeviceLinkRPCV2;


bool PerformAudioPassThru_requestMarshaller::checkIntegrity(PerformAudioPassThru_request& s)
{
  return checkIntegrityConst(s);
}


bool PerformAudioPassThru_requestMarshaller::fromString(const std::string& s,PerformAudioPassThru_request& e)
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


const std::string PerformAudioPassThru_requestMarshaller::toString(const PerformAudioPassThru_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool PerformAudioPassThru_requestMarshaller::checkIntegrityConst(const PerformAudioPassThru_request& s)
{
  {
    unsigned int i=s.initialPrompt.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TTSChunkMarshaller::checkIntegrityConst(s.initialPrompt[i]))   return false;
    }
  }
  if(s.audioPassThruDisplayText1 && s.audioPassThruDisplayText1->length()>500)  return false;
  if(s.audioPassThruDisplayText2 && s.audioPassThruDisplayText2->length()>500)  return false;
  if(!SamplingRateMarshaller::checkIntegrityConst(s.samplingRate))  return false;
  if(s.maxDuration>1000000)  return false;
  if(!AudioCaptureQualityMarshaller::checkIntegrityConst(s.bitsPerSample))  return false;
  if(!AudioTypeMarshaller::checkIntegrityConst(s.audioType))  return false;
  return true;
}

Json::Value PerformAudioPassThru_requestMarshaller::toJSON(const PerformAudioPassThru_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["initialPrompt"]=Json::Value(Json::arrayValue);
  json["initialPrompt"].resize(e.initialPrompt.size());
  for(unsigned int i=0;i<e.initialPrompt.size();i++)
    json["initialPrompt"][i]=TTSChunkMarshaller::toJSON(e.initialPrompt[i]);

  if(e.audioPassThruDisplayText1)
    json["audioPassThruDisplayText1"]=Json::Value(*e.audioPassThruDisplayText1);

  if(e.audioPassThruDisplayText2)
    json["audioPassThruDisplayText2"]=Json::Value(*e.audioPassThruDisplayText2);

  json["samplingRate"]=SamplingRateMarshaller::toJSON(e.samplingRate);

  json["maxDuration"]=Json::Value(e.maxDuration);

  json["bitsPerSample"]=AudioCaptureQualityMarshaller::toJSON(e.bitsPerSample);

  json["audioType"]=AudioTypeMarshaller::toJSON(e.audioType);

  return json;
}


bool PerformAudioPassThru_requestMarshaller::fromJSON(const Json::Value& json,PerformAudioPassThru_request& c)
{
  if(c.audioPassThruDisplayText1)  delete c.audioPassThruDisplayText1;
  c.audioPassThruDisplayText1=0;

  if(c.audioPassThruDisplayText2)  delete c.audioPassThruDisplayText2;
  c.audioPassThruDisplayText2=0;

  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("initialPrompt"))  return false;
    {
      const Json::Value& j=json["initialPrompt"];
      if(!j.isArray())  return false;
      c.initialPrompt.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          TTSChunk t;
          if(!TTSChunkMarshaller::fromJSON(j[i],t))
            return false;
          c.initialPrompt[i]=t;
        }

    }
    if(json.isMember("audioPassThruDisplayText1"))
    {
      const Json::Value& j=json["audioPassThruDisplayText1"];
      if(!j.isString())  return false;
      c.audioPassThruDisplayText1=new std::string(j.asString());
    }
    if(json.isMember("audioPassThruDisplayText2"))
    {
      const Json::Value& j=json["audioPassThruDisplayText2"];
      if(!j.isString())  return false;
      c.audioPassThruDisplayText2=new std::string(j.asString());
    }
    if(!json.isMember("samplingRate"))  return false;
    {
      const Json::Value& j=json["samplingRate"];
      if(!SamplingRateMarshaller::fromJSON(j,c.samplingRate))
        return false;
    }
    if(!json.isMember("maxDuration"))  return false;
    {
      const Json::Value& j=json["maxDuration"];
      if(!j.isInt())  return false;
      c.maxDuration=j.asInt();
    }
    if(!json.isMember("bitsPerSample"))  return false;
    {
      const Json::Value& j=json["bitsPerSample"];
      if(!AudioCaptureQualityMarshaller::fromJSON(j,c.bitsPerSample))
        return false;
    }
    if(!json.isMember("audioType"))  return false;
    {
      const Json::Value& j=json["audioType"];
      if(!AudioTypeMarshaller::fromJSON(j,c.audioType))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

