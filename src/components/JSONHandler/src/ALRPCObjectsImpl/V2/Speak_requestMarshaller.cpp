#include "../include/JSONHandler/ALRPCObjects/V2/Speak_request.h"
#include "TTSChunkMarshaller.h"

#include "Speak_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool Speak_requestMarshaller::checkIntegrity(Speak_request& s)
{
  return checkIntegrityConst(s);
}


bool Speak_requestMarshaller::fromString(const std::string& s,Speak_request& e)
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


const std::string Speak_requestMarshaller::toString(const Speak_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool Speak_requestMarshaller::checkIntegrityConst(const Speak_request& s)
{
  {
    unsigned int i=s.ttsChunks.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TTSChunkMarshaller::checkIntegrityConst(s.ttsChunks[i]))   return false;
    }
  }
  return true;
}

Json::Value Speak_requestMarshaller::toJSON(const Speak_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["ttsChunks"]=Json::Value(Json::arrayValue);
  json["ttsChunks"].resize(e.ttsChunks.size());
  for(unsigned int i=0;i<e.ttsChunks.size();i++)
    json["ttsChunks"][i]=TTSChunkMarshaller::toJSON(e.ttsChunks[i]);

  return json;
}


bool Speak_requestMarshaller::fromJSON(const Json::Value& json,Speak_request& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("ttsChunks"))  return false;
    {
      const Json::Value& j=json["ttsChunks"];
      if(!j.isArray())  return false;
      c.ttsChunks.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          TTSChunk t;
          if(!TTSChunkMarshaller::fromJSON(j[i],t))
            return false;
          c.ttsChunks[i]=t;
        }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

