#include "../include/JSONHandler/SDLRPCObjects/V2/PresetBankCapabilities.h"


#include "PresetBankCapabilitiesMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsSmartDeviceLinkRPCV2;


bool PresetBankCapabilitiesMarshaller::checkIntegrity(PresetBankCapabilities& s)
{
  return checkIntegrityConst(s);
}


bool PresetBankCapabilitiesMarshaller::fromString(const std::string& s,PresetBankCapabilities& e)
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


const std::string PresetBankCapabilitiesMarshaller::toString(const PresetBankCapabilities& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool PresetBankCapabilitiesMarshaller::checkIntegrityConst(const PresetBankCapabilities& s)
{
  return true;
}

Json::Value PresetBankCapabilitiesMarshaller::toJSON(const PresetBankCapabilities& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["onScreenPresetsAvailable"]=Json::Value(e.onScreenPresetsAvailable);


  return json;
}


bool PresetBankCapabilitiesMarshaller::fromJSON(const Json::Value& json,PresetBankCapabilities& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("onScreenPresetsAvailable"))  return false;
    {
      const Json::Value& j=json["onScreenPresetsAvailable"];
      if(!j.isBool())  return false;
      c.onScreenPresetsAvailable=j.asBool();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

