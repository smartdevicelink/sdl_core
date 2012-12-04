#include "../include/JSONHandler/ALRPCObjects/V2/EndAudioPassThru_request.h"


#include "EndAudioPassThru_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool EndAudioPassThru_requestMarshaller::checkIntegrity(EndAudioPassThru_request& s)
{
  return checkIntegrityConst(s);
}


bool EndAudioPassThru_requestMarshaller::fromString(const std::string& s,EndAudioPassThru_request& e)
{
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(!fromJSON(json,e))  return false;
  }
  catch(...)
  {
    return false;
  }
  return true;
}


const std::string EndAudioPassThru_requestMarshaller::toString(const EndAudioPassThru_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool EndAudioPassThru_requestMarshaller::checkIntegrityConst(const EndAudioPassThru_request& s)
{
  return true;
}

Json::Value EndAudioPassThru_requestMarshaller::toJSON(const EndAudioPassThru_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  return json;
}


bool EndAudioPassThru_requestMarshaller::fromJSON(const Json::Value& json,EndAudioPassThru_request& c)
{
  try
  {
    if(!json.isObject())  return false;


  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

