//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "../include/JSONHandler/SDLRPCObjects/V2/ShowConstantTBT_request.h"
#include "ImageMarshaller.h"
#include "SoftButtonMarshaller.h"

#include "ShowConstantTBT_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


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

  if(e.navigationText1)
    json["navigationText1"]=Json::Value(*e.navigationText1);

  if(e.navigationText2)
    json["navigationText2"]=Json::Value(*e.navigationText2);

  if(e.eta)
    json["eta"]=Json::Value(*e.eta);

  if(e.totalDistance)
    json["totalDistance"]=Json::Value(*e.totalDistance);

  json["turnIcon"]=ImageMarshaller::toJSON(e.turnIcon);

  json["distanceToManeuver"]=Json::Value(e.distanceToManeuver);

  json["distanceToManeuverScale"]=Json::Value(e.distanceToManeuverScale);

  if(e.maneuverComplete)
    json["maneuverComplete"]=Json::Value(*e.maneuverComplete);

  json["softButtons"]=Json::Value(Json::arrayValue);
  json["softButtons"].resize(e.softButtons.size());
  for(unsigned int i=0;i<e.softButtons.size();i++)
    json["softButtons"][i]=SoftButtonMarshaller::toJSON(e.softButtons[i]);

  return json;
}


bool ShowConstantTBT_requestMarshaller::fromJSON(const Json::Value& json,ShowConstantTBT_request& c)
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
      if(!j.isNumeric())  return false;
      c.distanceToManeuver=j.asDouble();
    }
    if(!json.isMember("distanceToManeuverScale"))  return false;
    {
      const Json::Value& j=json["distanceToManeuverScale"];
      if(!j.isNumeric())  return false;
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

