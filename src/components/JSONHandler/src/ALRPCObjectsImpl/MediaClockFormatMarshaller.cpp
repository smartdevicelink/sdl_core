#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/MediaClockFormat.h"
#include "MediaClockFormatMarshaller.h"
#include "MediaClockFormatMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 10:37:06 2012
  source stamp	Mon Nov 19 10:35:56 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


const MediaClockFormat::MediaClockFormatInternal MediaClockFormatMarshaller::getIndex(const char* s)
{
  if(!s)
    return MediaClockFormat::INVALID_ENUM;
  const struct PerfectHashTable* p=MediaClockFormat_intHash::getPointer(s,strlen(s));
  return p ? static_cast<MediaClockFormat::MediaClockFormatInternal>(p->idx) : MediaClockFormat::INVALID_ENUM;
}


bool MediaClockFormatMarshaller::fromJSON(const Json::Value& s,MediaClockFormat& e)
{
  e.mInternal=MediaClockFormat::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=MediaClockFormat::INVALID_ENUM);
}


Json::Value MediaClockFormatMarshaller::toJSON(const MediaClockFormat& e)
{
  if(e.mInternal==MediaClockFormat::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool MediaClockFormatMarshaller::fromString(const std::string& s,MediaClockFormat& e)
{
  e.mInternal=MediaClockFormat::INVALID_ENUM;
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

const std::string MediaClockFormatMarshaller::toString(const MediaClockFormat& e)
{
  Json::FastWriter writer;
  return e.mInternal==MediaClockFormat::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable MediaClockFormatMarshaller::mHashTable[5]=
{
  {"CLOCK1",0},
  {"CLOCK2",1},
  {"CLOCKTEXT1",2},
  {"CLOCKTEXT2",3},
  {"CLOCKTEXT3",4}
};
