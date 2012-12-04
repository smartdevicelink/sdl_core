#include "../include/JSONHandler/ALRPCObjects/V1/Choice.h"


#include "ChoiceMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV1;


bool ChoiceMarshaller::checkIntegrity(Choice& s)
{
  return checkIntegrityConst(s);
}


bool ChoiceMarshaller::fromString(const std::string& s,Choice& e)
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


const std::string ChoiceMarshaller::toString(const Choice& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ChoiceMarshaller::checkIntegrityConst(const Choice& s)
{
  if(s.choiceID>65535)  return false;
  if(s.menuName.length()>500)  return false;
  {
    unsigned int i=s.vrCommands.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
      if(s.vrCommands[i].length()>99)  return false;
    }
  }
  return true;
}

Json::Value ChoiceMarshaller::toJSON(const Choice& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["choiceID"]=Json::Value(e.choiceID);

  json["menuName"]=Json::Value(e.menuName);

  json["vrCommands"]=Json::Value(Json::arrayValue);
  json["vrCommands"].resize(e.vrCommands.size());
  for(unsigned int i=0;i<e.vrCommands.size();i++)
    json["vrCommands"][i]=Json::Value(e.vrCommands[i]);


  return json;
}


bool ChoiceMarshaller::fromJSON(const Json::Value& json,Choice& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("choiceID"))  return false;
    {
      const Json::Value& j=json["choiceID"];
      if(!j.isInt())  return false;
      c.choiceID=j.asInt();
    }
    if(!json.isMember("menuName"))  return false;
    {
      const Json::Value& j=json["menuName"];
      if(!j.isString())  return false;
      c.menuName=j.asString();
    }
    if(!json.isMember("vrCommands"))  return false;
    {
      const Json::Value& j=json["vrCommands"];
      if(!j.isArray())  return false;
      c.vrCommands.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        if(!j[i].isString())
          return false;
        else
          c.vrCommands[i]=j[i].asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

