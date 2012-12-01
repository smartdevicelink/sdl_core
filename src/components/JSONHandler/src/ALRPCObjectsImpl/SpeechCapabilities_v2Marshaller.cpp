#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/SpeechCapabilities_v2.h"
#include "SpeechCapabilities_v2Marshaller.h"
#include "SpeechCapabilities_v2Marshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


const SpeechCapabilities_v2::SpeechCapabilities_v2Internal SpeechCapabilities_v2Marshaller::getIndex(const char* s)
{
  if(!s)
    return SpeechCapabilities_v2::INVALID_ENUM;
  const struct PerfectHashTable* p=SpeechCapabilities_v2_intHash::getPointer(s,strlen(s));
  return p ? static_cast<SpeechCapabilities_v2::SpeechCapabilities_v2Internal>(p->idx) : SpeechCapabilities_v2::INVALID_ENUM;
}


bool SpeechCapabilities_v2Marshaller::fromJSON(const Json::Value& s,SpeechCapabilities_v2& e)
{
  e.mInternal=SpeechCapabilities_v2::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=SpeechCapabilities_v2::INVALID_ENUM);
}


Json::Value SpeechCapabilities_v2Marshaller::toJSON(const SpeechCapabilities_v2& e)
{
  if(e.mInternal==SpeechCapabilities_v2::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool SpeechCapabilities_v2Marshaller::fromString(const std::string& s,SpeechCapabilities_v2& e)
{
  e.mInternal=SpeechCapabilities_v2::INVALID_ENUM;
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(fromJSON(json,e))  return true;
  }
  catch(...)
  {
    return false;
  }
  return false;
}

const std::string SpeechCapabilities_v2Marshaller::toString(const SpeechCapabilities_v2& e)
{
  Json::FastWriter writer;
  return e.mInternal==SpeechCapabilities_v2::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable SpeechCapabilities_v2Marshaller::mHashTable[5]=
{
  {"TEXT",0},
  {"SAPI_PHONEMES",1},
  {"LHPLUS_PHONEMES",2},
  {"PRE_RECORDED",3},
  {"SILENCE",4}
};
