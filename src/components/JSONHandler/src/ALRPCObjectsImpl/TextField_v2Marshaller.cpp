#include "../include/JSONHandler/ALRPCObjects/TextField_v2.h"
#include "CharacterSetMarshaller.h"
#include "TextFieldName_v2Marshaller.h"

#include "TextField_v2Marshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool TextField_v2Marshaller::checkIntegrity(TextField_v2& s)
{
  return checkIntegrityConst(s);
}


bool TextField_v2Marshaller::fromString(const std::string& s,TextField_v2& e)
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


const std::string TextField_v2Marshaller::toString(const TextField_v2& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool TextField_v2Marshaller::checkIntegrityConst(const TextField_v2& s)
{
  if(!CharacterSetMarshaller::checkIntegrityConst(s.characterSet))  return false;
  if(!TextFieldName_v2Marshaller::checkIntegrityConst(s.name))  return false;
  if(s.rows>3)  return false;
  if(s.rows<1)  return false;
  if(s.width>500)  return false;
  if(s.width<1)  return false;
  return true;
}

Json::Value TextField_v2Marshaller::toJSON(const TextField_v2& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["characterSet"]=CharacterSetMarshaller::toJSON(e.characterSet);

  json["name"]=TextFieldName_v2Marshaller::toJSON(e.name);

  json["rows"]=Json::Value(e.rows);

  json["width"]=Json::Value(e.width);


  return json;
}


bool TextField_v2Marshaller::fromJSON(const Json::Value& json,TextField_v2& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("characterSet"))  return false;
    {
      const Json::Value& j=json["characterSet"];
      if(!CharacterSetMarshaller::fromJSON(j,c.characterSet))
        return false;
    }
    if(!json.isMember("name"))  return false;
    {
      const Json::Value& j=json["name"];
      if(!TextFieldName_v2Marshaller::fromJSON(j,c.name))
        return false;
    }
    if(!json.isMember("rows"))  return false;
    {
      const Json::Value& j=json["rows"];
      if(!j.isInt())  return false;
      c.rows=j.asInt();
    }
    if(!json.isMember("width"))  return false;
    {
      const Json::Value& j=json["width"];
      if(!j.isInt())  return false;
      c.width=j.asInt();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

