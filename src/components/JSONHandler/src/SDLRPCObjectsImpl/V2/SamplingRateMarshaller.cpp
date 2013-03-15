//
// Copyright (c) 2013 Ford Motor Company
//

#include <cstring>
#include "../include/JSONHandler/SDLRPCObjects/V2/SamplingRate.h"
#include "SamplingRateMarshaller.h"
#include "SamplingRateMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


const SamplingRate::SamplingRateInternal SamplingRateMarshaller::getIndex(const char* s)
{
  if(!s)
    return SamplingRate::INVALID_ENUM;
  const struct PerfectHashTable* p=SamplingRate_intHash::getPointer(s,strlen(s));
  return p ? static_cast<SamplingRate::SamplingRateInternal>(p->idx) : SamplingRate::INVALID_ENUM;
}


bool SamplingRateMarshaller::fromJSON(const Json::Value& s,SamplingRate& e)
{
  e.mInternal=SamplingRate::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=SamplingRate::INVALID_ENUM);
}


Json::Value SamplingRateMarshaller::toJSON(const SamplingRate& e)
{
  if(e.mInternal==SamplingRate::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool SamplingRateMarshaller::fromString(const std::string& s,SamplingRate& e)
{
  e.mInternal=SamplingRate::INVALID_ENUM;
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

const std::string SamplingRateMarshaller::toString(const SamplingRate& e)
{
  Json::FastWriter writer;
  return e.mInternal==SamplingRate::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable SamplingRateMarshaller::mHashTable[4]=
{
  {"8KHZ",0},
  {"16KHZ",1},
  {"22KHZ",2},
  {"44KHZ",3}
};
