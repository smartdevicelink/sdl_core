#include "../include/JSONHandler/ALRPCObjects/V2/OnTBTClientState.h"
#include "TBTStateMarshaller.h"

#include "OnTBTClientStateMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool OnTBTClientStateMarshaller::checkIntegrity(OnTBTClientState& s)
{
  return checkIntegrityConst(s);
}


bool OnTBTClientStateMarshaller::fromString(const std::string& s,OnTBTClientState& e)
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


const std::string OnTBTClientStateMarshaller::toString(const OnTBTClientState& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnTBTClientStateMarshaller::checkIntegrityConst(const OnTBTClientState& s)
{
  if(!TBTStateMarshaller::checkIntegrityConst(s.state))  return false;
  return true;
}

Json::Value OnTBTClientStateMarshaller::toJSON(const OnTBTClientState& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["state"]=TBTStateMarshaller::toJSON(e.state);

  return json;
}


bool OnTBTClientStateMarshaller::fromJSON(const Json::Value& json,OnTBTClientState& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("state"))  return false;
    {
      const Json::Value& j=json["state"];
      if(!TBTStateMarshaller::fromJSON(j,c.state))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

