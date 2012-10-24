#include <cstring>
#include "../../include/JSONHandler/ALRPCObjects/messageType.h"
#include "messageTypeMarshaller.h"
#include "messageTypeMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/



const messageType::messageTypeInternal messageTypeMarshaller::getIndex(const char* s)
{
  if(!s)
    return messageType::INVALID_ENUM;
  const struct PerfectHashTable* p=messageType_intHash::getPointer(s,strlen(s));
  return p ? static_cast<messageType::messageTypeInternal>(p->idx) : messageType::INVALID_ENUM;
}


bool messageTypeMarshaller::fromJSON(const Json::Value& s,messageType& e)
{
  e.mInternal=messageType::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=messageType::INVALID_ENUM);
}


Json::Value messageTypeMarshaller::toJSON(const messageType& e)
{
  if(e.mInternal==messageType::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool messageTypeMarshaller::fromString(const std::string& s,messageType& e)
{
  e.mInternal=messageType::INVALID_ENUM;
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

const std::string messageTypeMarshaller::toString(const messageType& e)
{
  Json::FastWriter writer;
  return e.mInternal==messageType::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable messageTypeMarshaller::mHashTable[3]=
{
  {"request",0},
  {"response",1},
  {"notification",2}
};
