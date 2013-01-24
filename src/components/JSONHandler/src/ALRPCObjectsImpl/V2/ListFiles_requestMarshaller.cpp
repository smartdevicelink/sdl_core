#include "../include/JSONHandler/ALRPCObjects/V2/ListFiles_request.h"


#include "ListFiles_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool ListFiles_requestMarshaller::checkIntegrity(ListFiles_request& s)
{
  return checkIntegrityConst(s);
}


bool ListFiles_requestMarshaller::fromString(const std::string& s,ListFiles_request& e)
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


const std::string ListFiles_requestMarshaller::toString(const ListFiles_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ListFiles_requestMarshaller::checkIntegrityConst(const ListFiles_request& s)
{
  return true;
}

Json::Value ListFiles_requestMarshaller::toJSON(const ListFiles_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  return json;
}


bool ListFiles_requestMarshaller::fromJSON(const Json::Value& json,ListFiles_request& c)
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

