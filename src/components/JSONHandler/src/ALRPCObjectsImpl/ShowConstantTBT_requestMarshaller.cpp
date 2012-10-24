#include "../../include/JSONHandler/ALRPCObjects/ShowConstantTBT_request.h"
#include "ImageMarshaller.h"
#include "SoftButtonMarshaller.h"

#include "ShowConstantTBT_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


bool ShowConstantTBT_requestMarshaller::checkIntegrity(ShowConstantTBT_request& s)
{
  return checkIntegrityConst(s);
}


bool ShowConstantTBT_requestMarshaller::fromString(const std::string& s,ShowConstantTBT_request& e)
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


const std::string ShowConstantTBT_requestMarshaller::toString(const ShowConstantTBT_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ShowConstantTBT_requestMarshaller::checkIntegrityConst(const ShowConstantTBT_request& s)
{
  if(s.navigationText1 && s.navigationText1->length()>500)  return false;
  if(s.navigationText2 && s.navigationText2->length()>500)  return false;
  if(s.eta && s.eta->length()>500)  return false;
  if(s.totalDistance && s.totalDistance->length()>500)  return false;
  if(!ImageMarshaller::checkIntegrityConst(s.turnIcon))  return false;
  if(s.distanceToManeuver>1000000000)  return false;
  if(s.distanceToManeuver<0)  return false;
  if(s.distanceToManeuverScale>1000000000)  return false;
  if(s.distanceToManeuverScale<0)  return false;
  {
    unsigned int i=s.softButtons.size();
    if(i>3 || i<0)  return false;
    while(i--)
    {
    if(!SoftButtonMarshaller::checkIntegrityConst(s.softButtons[i]))   return false;
    }
  }
  return true;
}

Json::Value ShowConstantTBT_requestMarshaller::toJSON(const ShowConstantTBT_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("ShowConstantTBT");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  if(e.navigationText1)
    j["navigationText1"]=Json::Value(*e.navigationText1);

  if(e.navigationText2)
    j["navigationText2"]=Json::Value(*e.navigationText2);

  if(e.eta)
    j["eta"]=Json::Value(*e.eta);

  if(e.totalDistance)
    j["totalDistance"]=Json::Value(*e.totalDistance);

  j["turnIcon"]=ImageMarshaller::toJSON(e.turnIcon);

  j["distanceToManeuver"]=Json::Value(e.distanceToManeuver);

  j["distanceToManeuverScale"]=Json::Value(e.distanceToManeuverScale);

  if(e.maneuverComplete)
    j["maneuverComplete"]=Json::Value(*e.maneuverComplete);

  j["softButtons"]=Json::Value(Json::arrayValue);
  j["softButtons"].resize(e.softButtons.size());
  for(unsigned int i=0;i<e.softButtons.size();i++)
    j["softButtons"][i]=SoftButtonMarshaller::toJSON(e.softButtons[i]);

  json["request"]["parameters"]=j;
  return json;
}


bool ShowConstantTBT_requestMarshaller::fromJSON(const Json::Value& js,ShowConstantTBT_request& c)
{
  if(c.navigationText1)  delete c.navigationText1;
  c.navigationText1=0;

  if(c.navigationText2)  delete c.navigationText2;
  c.navigationText2=0;

  if(c.eta)  delete c.eta;
  c.eta=0;

  if(c.totalDistance)  delete c.totalDistance;
  c.totalDistance=0;

  if(c.maneuverComplete)  delete c.maneuverComplete;
  c.maneuverComplete=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("ShowConstantTBT"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(json.isMember("navigationText1"))
    {
      const Json::Value& j=json["navigationText1"];
      if(!j.isString())  return false;
      c.navigationText1=new std::string(j.asString());
    }
    if(json.isMember("navigationText2"))
    {
      const Json::Value& j=json["navigationText2"];
      if(!j.isString())  return false;
      c.navigationText2=new std::string(j.asString());
    }
    if(json.isMember("eta"))
    {
      const Json::Value& j=json["eta"];
      if(!j.isString())  return false;
      c.eta=new std::string(j.asString());
    }
    if(json.isMember("totalDistance"))
    {
      const Json::Value& j=json["totalDistance"];
      if(!j.isString())  return false;
      c.totalDistance=new std::string(j.asString());
    }
    if(!json.isMember("turnIcon"))  return false;
    {
      const Json::Value& j=json["turnIcon"];
      if(!ImageMarshaller::fromJSON(j,c.turnIcon))
        return false;
    }
    if(!json.isMember("distanceToManeuver"))  return false;
    {
      const Json::Value& j=json["distanceToManeuver"];
      if(!j.isDouble())  return false;
      c.distanceToManeuver=j.asDouble();
    }
    if(!json.isMember("distanceToManeuverScale"))  return false;
    {
      const Json::Value& j=json["distanceToManeuverScale"];
      if(!j.isDouble())  return false;
      c.distanceToManeuverScale=j.asDouble();
    }
    if(json.isMember("maneuverComplete"))
    {
      const Json::Value& j=json["maneuverComplete"];
      if(!j.isBool())  return false;
      c.maneuverComplete=new bool(j.asBool());
    }
    if(!json.isMember("softButtons"))  return false;
    {
      const Json::Value& j=json["softButtons"];
      if(!j.isArray())  return false;
      c.softButtons.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          SoftButton t;
          if(!SoftButtonMarshaller::fromJSON(j[i],t))
            return false;
          c.softButtons[i]=t;
        }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

