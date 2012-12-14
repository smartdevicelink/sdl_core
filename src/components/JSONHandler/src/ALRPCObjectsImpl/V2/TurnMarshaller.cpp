#include "../include/JSONHandler/ALRPCObjects/V2/Turn.h"
#include "ImageMarshaller.h"

#include "TurnMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool TurnMarshaller::checkIntegrity(Turn& s)
{
  return checkIntegrityConst(s);
}


bool TurnMarshaller::fromString(const std::string& s,Turn& e)
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


const std::string TurnMarshaller::toString(const Turn& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool TurnMarshaller::checkIntegrityConst(const Turn& s)
{
  if(s.navigationText && s.navigationText->length()>500)  return false;
  if(!ImageMarshaller::checkIntegrityConst(s.turnIcon))  return false;
  return true;
}

Json::Value TurnMarshaller::toJSON(const Turn& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  if(e.navigationText)
    json["navigationText"]=Json::Value(*e.navigationText);

  json["turnIcon"]=ImageMarshaller::toJSON(e.turnIcon);


  return json;
}


bool TurnMarshaller::fromJSON(const Json::Value& json,Turn& c)
{
  if(c.navigationText)  delete c.navigationText;
  c.navigationText=0;

  try
  {
    if(!json.isObject())  return false;

    if(json.isMember("navigationText"))
    {
      const Json::Value& j=json["navigationText"];
      if(!j.isString())  return false;
      c.navigationText=new std::string(j.asString());
    }
    if(!json.isMember("turnIcon"))  return false;
    {
      const Json::Value& j=json["turnIcon"];
      if(!ImageMarshaller::fromJSON(j,c.turnIcon))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

