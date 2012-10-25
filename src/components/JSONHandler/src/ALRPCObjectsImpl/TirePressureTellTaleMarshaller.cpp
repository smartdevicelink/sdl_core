#include <cstring>
#include "../../include/JSONHandler/ALRPCObjects/TirePressureTellTale.h"
#include "TirePressureTellTaleMarshaller.h"
#include "TirePressureTellTaleMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/



const TirePressureTellTale::TirePressureTellTaleInternal TirePressureTellTaleMarshaller::getIndex(const char* s)
{
  if(!s)
    return TirePressureTellTale::INVALID_ENUM;
  const struct PerfectHashTable* p=TirePressureTellTale_intHash::getPointer(s,strlen(s));
  return p ? static_cast<TirePressureTellTale::TirePressureTellTaleInternal>(p->idx) : TirePressureTellTale::INVALID_ENUM;
}


bool TirePressureTellTaleMarshaller::fromJSON(const Json::Value& s,TirePressureTellTale& e)
{
  e.mInternal=TirePressureTellTale::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=TirePressureTellTale::INVALID_ENUM);
}


Json::Value TirePressureTellTaleMarshaller::toJSON(const TirePressureTellTale& e)
{
  if(e.mInternal==TirePressureTellTale::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool TirePressureTellTaleMarshaller::fromString(const std::string& s,TirePressureTellTale& e)
{
  e.mInternal=TirePressureTellTale::INVALID_ENUM;
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

const std::string TirePressureTellTaleMarshaller::toString(const TirePressureTellTale& e)
{
  Json::FastWriter writer;
  return e.mInternal==TirePressureTellTale::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable TirePressureTellTaleMarshaller::mHashTable[3]=
{
  {"OFF",0},
  {"ON",1},
  {"FLASH",2}
};
