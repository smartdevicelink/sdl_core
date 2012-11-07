#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/DisplayType.h"
#include "DisplayTypeMarshaller.h"
#include "DisplayTypeMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

using namespace AppLinkRPC;


const DisplayType::DisplayTypeInternal DisplayTypeMarshaller::getIndex(const char* s)
{
  if(!s)
    return DisplayType::INVALID_ENUM;
  const struct PerfectHashTable* p=DisplayType_intHash::getPointer(s,strlen(s));
  return p ? static_cast<DisplayType::DisplayTypeInternal>(p->idx) : DisplayType::INVALID_ENUM;
}


bool DisplayTypeMarshaller::fromJSON(const Json::Value& s,DisplayType& e)
{
  e.mInternal=DisplayType::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=DisplayType::INVALID_ENUM);
}


Json::Value DisplayTypeMarshaller::toJSON(const DisplayType& e)
{
  if(e.mInternal==DisplayType::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool DisplayTypeMarshaller::fromString(const std::string& s,DisplayType& e)
{
  e.mInternal=DisplayType::INVALID_ENUM;
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

const std::string DisplayTypeMarshaller::toString(const DisplayType& e)
{
  Json::FastWriter writer;
  return e.mInternal==DisplayType::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable DisplayTypeMarshaller::mHashTable[9]=
{
  {"CID",0},
  {"TYPE2",1},
  {"TYPE5",2},
  {"NGN",3},
  {"GEN2_8_DMA",4},
  {"GEN2_6_DMA",5},
  {"MFD3",6},
  {"MFD4",7},
  {"MFD5",8}
};
