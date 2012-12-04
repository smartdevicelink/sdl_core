#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/V2/TirePressureTellTale.h"
#include "TirePressureTellTaleMarshaller.h"
#include "TirePressureTellTaleMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


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
