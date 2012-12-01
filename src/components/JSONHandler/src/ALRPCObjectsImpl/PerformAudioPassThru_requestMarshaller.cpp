#include "../include/JSONHandler/ALRPCObjects/PerformAudioPassThru_request.h"
#include "AudioCaptureQuality_v2Marshaller.h"
#include "AudioTypeMarshaller.h"
#include "SamplingRateMarshaller.h"
#include "TTSChunk_v2Marshaller.h"

#include "PerformAudioPassThru_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


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
    if(!TTSChunk_v2Marshaller::checkIntegrityConst(s.initialPrompt[i]))   return false;
    }
  }
  if(s.audioPassThruDisplayText1 && s.audioPassThruDisplayText1->length()>500)  return false;
  if(s.audioPassThruDisplayText2 && s.audioPassThruDisplayText2->length()>500)  return false;
  if(s.samplingRate && !SamplingRateMarshaller::checkIntegrityConst(*s.samplingRate))  return false;
  if(s.maxDuration && *s.maxDuration>1000000)  return false;
  if(s.bitsPerSample && !AudioCaptureQuality_v2Marshaller::checkIntegrityConst(*s.bitsPerSample))  return false;
  if(s.audioType && !AudioTypeMarshaller::checkIntegrityConst(*s.audioType))  return false;
  return true;
}

Json::Value PerformAudioPassThru_requestMarshaller::toJSON(const PerformAudioPassThru_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("PerformAudioPassThru");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["initialPrompt"]=Json::Value(Json::arrayValue);
  j["initialPrompt"].resize(e.initialPrompt.size());
  for(unsigned int i=0;i<e.initialPrompt.size();i++)
    j["initialPrompt"][i]=TTSChunk_v2Marshaller::toJSON(e.initialPrompt[i]);

  if(e.audioPassThruDisplayText1)
    j["audioPassThruDisplayText1"]=Json::Value(*e.audioPassThruDisplayText1);

  if(e.audioPassThruDisplayText2)
    j["audioPassThruDisplayText2"]=Json::Value(*e.audioPassThruDisplayText2);

  if(e.samplingRate)
    j["samplingRate"]=SamplingRateMarshaller::toJSON(*e.samplingRate);

  if(e.maxDuration)
    j["maxDuration"]=Json::Value(*e.maxDuration);

  if(e.bitsPerSample)
    j["bitsPerSample"]=AudioCaptureQuality_v2Marshaller::toJSON(*e.bitsPerSample);

  if(e.audioType)
    j["audioType"]=AudioTypeMarshaller::toJSON(*e.audioType);

  json["request"]["parameters"]=j;
  return json;
}


bool PerformAudioPassThru_requestMarshaller::fromJSON(const Json::Value& js,PerformAudioPassThru_request& c)
{
  if(c.audioPassThruDisplayText1)  delete c.audioPassThruDisplayText1;
  c.audioPassThruDisplayText1=0;

  if(c.audioPassThruDisplayText2)  delete c.audioPassThruDisplayText2;
  c.audioPassThruDisplayText2=0;

  if(c.samplingRate)  delete c.samplingRate;
  c.samplingRate=0;

  if(c.maxDuration)  delete c.maxDuration;
  c.maxDuration=0;

  if(c.bitsPerSample)  delete c.bitsPerSample;
  c.bitsPerSample=0;

  if(c.audioType)  delete c.audioType;
  c.audioType=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("PerformAudioPassThru"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("initialPrompt"))  return false;
    {
      const Json::Value& j=json["initialPrompt"];
      if(!j.isArray())  return false;
      c.initialPrompt.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          TTSChunk_v2 t;
          if(!TTSChunk_v2Marshaller::fromJSON(j[i],t))
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
    if(json.isMember("samplingRate"))
    {
      const Json::Value& j=json["samplingRate"];
      c.samplingRate=new SamplingRate();
      if(!SamplingRateMarshaller::fromJSON(j,c.samplingRate[0]))
        return false;
    }
    if(json.isMember("maxDuration"))
    {
      const Json::Value& j=json["maxDuration"];
      if(!j.isInt())  return false;
      c.maxDuration=new unsigned int(j.asInt());
    }
    if(json.isMember("bitsPerSample"))
    {
      const Json::Value& j=json["bitsPerSample"];
      c.bitsPerSample=new AudioCaptureQuality_v2();
      if(!AudioCaptureQuality_v2Marshaller::fromJSON(j,c.bitsPerSample[0]))
        return false;
    }
    if(json.isMember("audioType"))
    {
      const Json::Value& j=json["audioType"];
      c.audioType=new AudioType();
      if(!AudioTypeMarshaller::fromJSON(j,c.audioType[0]))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

