//
// Copyright (c) 2013 Ford Motor Company
//

#include <cstring>
#include "../include/JSONHandler/SDLRPCObjects/V2/MediaClockFormat.h"
#include "MediaClockFormatMarshaller.h"
#include "MediaClockFormatMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


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
