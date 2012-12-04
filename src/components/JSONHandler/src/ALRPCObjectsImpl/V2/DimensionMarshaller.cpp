#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/V2/Dimension.h"
#include "DimensionMarshaller.h"
#include "DimensionMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


const Dimension::DimensionInternal DimensionMarshaller::getIndex(const char* s)
{
  if(!s)
    return Dimension::INVALID_ENUM;
  const struct PerfectHashTable* p=Dimension_intHash::getPointer(s,strlen(s));
  return p ? static_cast<Dimension::DimensionInternal>(p->idx) : Dimension::INVALID_ENUM;
}


bool DimensionMarshaller::fromJSON(const Json::Value& s,Dimension& e)
{
  e.mInternal=Dimension::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=Dimension::INVALID_ENUM);
}


Json::Value DimensionMarshaller::toJSON(const Dimension& e)
{
  if(e.mInternal==Dimension::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool DimensionMarshaller::fromString(const std::string& s,Dimension& e)
{
  e.mInternal=Dimension::INVALID_ENUM;
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

const std::string DimensionMarshaller::toString(const Dimension& e)
{
  Json::FastWriter writer;
  return e.mInternal==Dimension::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable DimensionMarshaller::mHashTable[3]=
{
  {"NO_FIX",0},
  {"2D",1},
  {"3D",2}
};
