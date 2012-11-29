#include "../include/JSONHandler/ALRPCObjects/DisplayCapabilities_v2.h"
#include "DisplayTypeMarshaller.h"
#include "MediaClockFormatMarshaller.h"
#include "TextField_v2Marshaller.h"

#include "DisplayCapabilities_v2Marshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool DisplayCapabilities_v2Marshaller::checkIntegrity(DisplayCapabilities_v2& s)
{
  return checkIntegrityConst(s);
}


bool DisplayCapabilities_v2Marshaller::fromString(const std::string& s,DisplayCapabilities_v2& e)
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


const std::string DisplayCapabilities_v2Marshaller::toString(const DisplayCapabilities_v2& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool DisplayCapabilities_v2Marshaller::checkIntegrityConst(const DisplayCapabilities_v2& s)
{
  if(!DisplayTypeMarshaller::checkIntegrityConst(s.displayType))  return false;
  {
    unsigned int i=s.mediaClockFormats.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!MediaClockFormatMarshaller::checkIntegrityConst(s.mediaClockFormats[i]))   return false;
    }
  }
  {
    unsigned int i=s.textFields.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TextField_v2Marshaller::checkIntegrityConst(s.textFields[i]))   return false;
    }
  }
  return true;
}

Json::Value DisplayCapabilities_v2Marshaller::toJSON(const DisplayCapabilities_v2& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["displayType"]=DisplayTypeMarshaller::toJSON(e.displayType);

  json["mediaClockFormats"]=Json::Value(Json::arrayValue);
  json["mediaClockFormats"].resize(e.mediaClockFormats.size());
  for(unsigned int i=0;i<e.mediaClockFormats.size();i++)
    json["mediaClockFormats"][i]=MediaClockFormatMarshaller::toJSON(e.mediaClockFormats[i]);

  json["textFields"]=Json::Value(Json::arrayValue);
  json["textFields"].resize(e.textFields.size());
  for(unsigned int i=0;i<e.textFields.size();i++)
    json["textFields"][i]=TextField_v2Marshaller::toJSON(e.textFields[i]);


  return json;
}


bool DisplayCapabilities_v2Marshaller::fromJSON(const Json::Value& json,DisplayCapabilities_v2& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("displayType"))  return false;
    {
      const Json::Value& j=json["displayType"];
      if(!DisplayTypeMarshaller::fromJSON(j,c.displayType))
        return false;
    }
    if(!json.isMember("mediaClockFormats"))  return false;
    {
      const Json::Value& j=json["mediaClockFormats"];
      if(!j.isArray())  return false;
      c.mediaClockFormats.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          MediaClockFormat t;
          if(!MediaClockFormatMarshaller::fromJSON(j[i],t))
            return false;
          c.mediaClockFormats[i]=t;
        }

    }
    if(!json.isMember("textFields"))  return false;
    {
      const Json::Value& j=json["textFields"];
      if(!j.isArray())  return false;
      c.textFields.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          TextField_v2 t;
          if(!TextField_v2Marshaller::fromJSON(j[i],t))
            return false;
          c.textFields[i]=t;
        }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

