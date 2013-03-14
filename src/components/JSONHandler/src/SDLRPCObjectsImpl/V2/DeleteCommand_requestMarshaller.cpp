#include "../include/JSONHandler/SDLRPCObjects/V2/DeleteCommand_request.h"


#include "DeleteCommand_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsSmartDeviceLinkRPCV2;


bool DeleteCommand_requestMarshaller::checkIntegrity(DeleteCommand_request& s)
{
  return checkIntegrityConst(s);
}


bool DeleteCommand_requestMarshaller::fromString(const std::string& s,DeleteCommand_request& e)
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


const std::string DeleteCommand_requestMarshaller::toString(const DeleteCommand_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool DeleteCommand_requestMarshaller::checkIntegrityConst(const DeleteCommand_request& s)
{
  if(s.cmdID>2000000000)  return false;
  return true;
}

Json::Value DeleteCommand_requestMarshaller::toJSON(const DeleteCommand_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["cmdID"]=Json::Value(e.cmdID);

  return json;
}


bool DeleteCommand_requestMarshaller::fromJSON(const Json::Value& json,DeleteCommand_request& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("cmdID"))  return false;
    {
      const Json::Value& j=json["cmdID"];
      if(!j.isInt())  return false;
      c.cmdID=j.asInt();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

