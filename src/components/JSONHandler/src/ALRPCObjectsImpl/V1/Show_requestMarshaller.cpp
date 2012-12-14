#include "../include/JSONHandler/ALRPCObjects/V1/Show_request.h"
#include "TextAlignmentMarshaller.h"

#include "Show_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool Show_requestMarshaller::checkIntegrity(Show_request& s)
{
  return checkIntegrityConst(s);
}


bool Show_requestMarshaller::fromString(const std::string& s,Show_request& e)
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


const std::string Show_requestMarshaller::toString(const Show_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool Show_requestMarshaller::checkIntegrityConst(const Show_request& s)
{
  if(s.mainField1 && s.mainField1->length()>500)  return false;
  if(s.mainField2 && s.mainField2->length()>500)  return false;
  if(s.alignment && !TextAlignmentMarshaller::checkIntegrityConst(*s.alignment))  return false;
  if(s.statusBar && s.statusBar->length()>500)  return false;
  if(s.mediaClock && s.mediaClock->length()>500)  return false;
  if(s.mediaTrack && s.mediaTrack->length()>500)  return false;
  return true;
}

Json::Value Show_requestMarshaller::toJSON(const Show_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("Show");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  if(e.mainField1)
    j["mainField1"]=Json::Value(*e.mainField1);

  if(e.mainField2)
    j["mainField2"]=Json::Value(*e.mainField2);

  if(e.alignment)
    j["alignment"]=TextAlignmentMarshaller::toJSON(*e.alignment);

  if(e.statusBar)
    j["statusBar"]=Json::Value(*e.statusBar);

  if(e.mediaClock)
    j["mediaClock"]=Json::Value(*e.mediaClock);

  if(e.mediaTrack)
    j["mediaTrack"]=Json::Value(*e.mediaTrack);

  json["request"]["parameters"]=j;
  return json;
}


bool Show_requestMarshaller::fromJSON(const Json::Value& js,Show_request& c)
{
  if(c.mainField1)  delete c.mainField1;
  c.mainField1=0;

  if(c.mainField2)  delete c.mainField2;
  c.mainField2=0;

  if(c.alignment)  delete c.alignment;
  c.alignment=0;

  if(c.statusBar)  delete c.statusBar;
  c.statusBar=0;

  if(c.mediaClock)  delete c.mediaClock;
  c.mediaClock=0;

  if(c.mediaTrack)  delete c.mediaTrack;
  c.mediaTrack=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("Show"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(json.isMember("mainField1"))
    {
      const Json::Value& j=json["mainField1"];
      if(!j.isString())  return false;
      c.mainField1=new std::string(j.asString());
    }
    if(json.isMember("mainField2"))
    {
      const Json::Value& j=json["mainField2"];
      if(!j.isString())  return false;
      c.mainField2=new std::string(j.asString());
    }
    if(json.isMember("alignment"))
    {
      const Json::Value& j=json["alignment"];
      c.alignment=new TextAlignment();
      if(!TextAlignmentMarshaller::fromJSON(j,c.alignment[0]))
        return false;
    }
    if(json.isMember("statusBar"))
    {
      const Json::Value& j=json["statusBar"];
      if(!j.isString())  return false;
      c.statusBar=new std::string(j.asString());
    }
    if(json.isMember("mediaClock"))
    {
      const Json::Value& j=json["mediaClock"];
      if(!j.isString())  return false;
      c.mediaClock=new std::string(j.asString());
    }
    if(json.isMember("mediaTrack"))
    {
      const Json::Value& j=json["mediaTrack"];
      if(!j.isString())  return false;
      c.mediaTrack=new std::string(j.asString());
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

