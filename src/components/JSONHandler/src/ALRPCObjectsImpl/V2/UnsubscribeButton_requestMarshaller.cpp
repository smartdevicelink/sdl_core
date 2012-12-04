#include "../include/JSONHandler/ALRPCObjects/V2/UnsubscribeButton_request.h"
#include "ButtonNameMarshaller.h"

#include "UnsubscribeButton_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool UnsubscribeButton_requestMarshaller::checkIntegrity(UnsubscribeButton_request& s)
{
  return checkIntegrityConst(s);
}


bool UnsubscribeButton_requestMarshaller::fromString(const std::string& s,UnsubscribeButton_request& e)
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


const std::string UnsubscribeButton_requestMarshaller::toString(const UnsubscribeButton_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool UnsubscribeButton_requestMarshaller::checkIntegrityConst(const UnsubscribeButton_request& s)
{
  if(!ButtonNameMarshaller::checkIntegrityConst(s.buttonName))  return false;
  return true;
}

Json::Value UnsubscribeButton_requestMarshaller::toJSON(const UnsubscribeButton_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["buttonName"]=ButtonNameMarshaller::toJSON(e.buttonName);

  return json;
}


bool UnsubscribeButton_requestMarshaller::fromJSON(const Json::Value& json,UnsubscribeButton_request& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("buttonName"))  return false;
    {
      const Json::Value& j=json["buttonName"];
      if(!ButtonNameMarshaller::fromJSON(j,c.buttonName))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

