#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/FileType.h"
#include "FileTypeMarshaller.h"
#include "FileTypeMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


const FileType::FileTypeInternal FileTypeMarshaller::getIndex(const char* s)
{
  if(!s)
    return FileType::INVALID_ENUM;
  const struct PerfectHashTable* p=FileType_intHash::getPointer(s,strlen(s));
  return p ? static_cast<FileType::FileTypeInternal>(p->idx) : FileType::INVALID_ENUM;
}


bool FileTypeMarshaller::fromJSON(const Json::Value& s,FileType& e)
{
  e.mInternal=FileType::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=FileType::INVALID_ENUM);
}


Json::Value FileTypeMarshaller::toJSON(const FileType& e)
{
  if(e.mInternal==FileType::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool FileTypeMarshaller::fromString(const std::string& s,FileType& e)
{
  e.mInternal=FileType::INVALID_ENUM;
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

const std::string FileTypeMarshaller::toString(const FileType& e)
{
  Json::FastWriter writer;
  return e.mInternal==FileType::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable FileTypeMarshaller::mHashTable[5]=
{
  {"GRAPHIC_BMP",0},
  {"GRAPHIC_JPEG",1},
  {"GRAPHIC_PNG",2},
  {"AUDIO_WAVE",3},
  {"AUDIO_MP3",4}
};
