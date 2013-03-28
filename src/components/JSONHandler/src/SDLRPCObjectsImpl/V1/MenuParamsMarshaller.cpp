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

#include "../include/JSONHandler/SDLRPCObjects/V1/MenuParams.h"


#include "MenuParamsMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPC;


bool MenuParamsMarshaller::checkIntegrity(MenuParams& s)
{
  return checkIntegrityConst(s);
}


bool MenuParamsMarshaller::fromString(const std::string& s,MenuParams& e)
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


const std::string MenuParamsMarshaller::toString(const MenuParams& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool MenuParamsMarshaller::checkIntegrityConst(const MenuParams& s)
{
  if(s.menuName.length()>500)  return false;
  if(s.parentID && *s.parentID>2000000000)  return false;
  if(s.position && *s.position>1000)  return false;
  return true;
}

Json::Value MenuParamsMarshaller::toJSON(const MenuParams& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["menuName"]=Json::Value(e.menuName);

  if(e.parentID)
    json["parentID"]=Json::Value(*e.parentID);

  if(e.position)
    json["position"]=Json::Value(*e.position);


  return json;
}


bool MenuParamsMarshaller::fromJSON(const Json::Value& json,MenuParams& c)
{
  if(c.parentID)  delete c.parentID;
  c.parentID=0;

  if(c.position)  delete c.position;
  c.position=0;

  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("menuName"))  return false;
    {
      const Json::Value& j=json["menuName"];
      if(!j.isString())  return false;
      c.menuName=j.asString();
    }
    if(json.isMember("parentID"))
    {
      const Json::Value& j=json["parentID"];
      if(!j.isInt())  return false;
      c.parentID=new unsigned int(j.asInt());
    }
    if(json.isMember("position"))
    {
      const Json::Value& j=json["position"];
      if(!j.isInt())  return false;
      c.position=new unsigned int(j.asInt());
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

