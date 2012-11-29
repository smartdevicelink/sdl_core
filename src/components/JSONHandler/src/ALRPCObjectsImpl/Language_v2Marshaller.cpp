#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/Language_v2.h"
#include "Language_v2Marshaller.h"
#include "Language_v2Marshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


const Language_v2::Language_v2Internal Language_v2Marshaller::getIndex(const char* s)
{
  if(!s)
    return Language_v2::INVALID_ENUM;
  const struct PerfectHashTable* p=Language_v2_intHash::getPointer(s,strlen(s));
  return p ? static_cast<Language_v2::Language_v2Internal>(p->idx) : Language_v2::INVALID_ENUM;
}


bool Language_v2Marshaller::fromJSON(const Json::Value& s,Language_v2& e)
{
  e.mInternal=Language_v2::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=Language_v2::INVALID_ENUM);
}


Json::Value Language_v2Marshaller::toJSON(const Language_v2& e)
{
  if(e.mInternal==Language_v2::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool Language_v2Marshaller::fromString(const std::string& s,Language_v2& e)
{
  e.mInternal=Language_v2::INVALID_ENUM;
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

const std::string Language_v2Marshaller::toString(const Language_v2& e)
{
  Json::FastWriter writer;
  return e.mInternal==Language_v2::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable Language_v2Marshaller::mHashTable[20]=
{
  {"EN-US",0},
  {"ES-MX",1},
  {"FR-CA",2},
  {"DE-EU",3},
  {"ES-EU",4},
  {"EN-EU",5},
  {"RU-RU",6},
  {"TR-TR",7},
  {"PL-EU",8},
  {"FR-EU",9},
  {"IT-EU",10},
  {"SV-EU",11},
  {"PT-EU",12},
  {"NL-EU",13},
  {"EN-AU",14},
  {"ZH-CN",15},
  {"ZH-TW",16},
  {"JA-JP",17},
  {"AR",18},
  {"KO-KR",19}
};
