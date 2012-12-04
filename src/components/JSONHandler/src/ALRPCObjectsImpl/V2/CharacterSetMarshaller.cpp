#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/V2/CharacterSet.h"
#include "CharacterSetMarshaller.h"
#include "CharacterSetMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


const CharacterSet::CharacterSetInternal CharacterSetMarshaller::getIndex(const char* s)
{
  if(!s)
    return CharacterSet::INVALID_ENUM;
  const struct PerfectHashTable* p=CharacterSet_intHash::getPointer(s,strlen(s));
  return p ? static_cast<CharacterSet::CharacterSetInternal>(p->idx) : CharacterSet::INVALID_ENUM;
}


bool CharacterSetMarshaller::fromJSON(const Json::Value& s,CharacterSet& e)
{
  e.mInternal=CharacterSet::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=CharacterSet::INVALID_ENUM);
}


Json::Value CharacterSetMarshaller::toJSON(const CharacterSet& e)
{
  if(e.mInternal==CharacterSet::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool CharacterSetMarshaller::fromString(const std::string& s,CharacterSet& e)
{
  e.mInternal=CharacterSet::INVALID_ENUM;
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

const std::string CharacterSetMarshaller::toString(const CharacterSet& e)
{
  Json::FastWriter writer;
  return e.mInternal==CharacterSet::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable CharacterSetMarshaller::mHashTable[4]=
{
  {"TYPE2SET",0},
  {"TYPE5SET",1},
  {"CID1SET",2},
  {"CID2SET",3}
};
