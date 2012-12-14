#include "../include/JSONHandler/ALRPCObjects/V2/UnregisterAppInterface_request.h"


#include "UnregisterAppInterface_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool UnregisterAppInterface_requestMarshaller::checkIntegrity(UnregisterAppInterface_request& s)
{
  return checkIntegrityConst(s);
}


bool UnregisterAppInterface_requestMarshaller::fromString(const std::string& s,UnregisterAppInterface_request& e)
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


const std::string UnregisterAppInterface_requestMarshaller::toString(const UnregisterAppInterface_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool UnregisterAppInterface_requestMarshaller::checkIntegrityConst(const UnregisterAppInterface_request& s)
{
  return true;
}

Json::Value UnregisterAppInterface_requestMarshaller::toJSON(const UnregisterAppInterface_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  return json;
}


bool UnregisterAppInterface_requestMarshaller::fromJSON(const Json::Value& json,UnregisterAppInterface_request& c)
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

