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

#include "../include/JSONHandler/SDLRPCObjects/V2/AddCommand_request.h"
#include "ImageMarshaller.h"
#include "MenuParamsMarshaller.h"

#include "AddCommand_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool AddCommand_requestMarshaller::checkIntegrity(AddCommand_request& s)
{
  return checkIntegrityConst(s);
}


bool AddCommand_requestMarshaller::fromString(const std::string& s,AddCommand_request& e)
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


const std::string AddCommand_requestMarshaller::toString(const AddCommand_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool AddCommand_requestMarshaller::checkIntegrityConst(const AddCommand_request& s)
{
  if(s.cmdID>2000000000)  return false;
  if(s.menuParams && !MenuParamsMarshaller::checkIntegrityConst(*s.menuParams))  return false;
  if(s.vrCommands)
  {
    unsigned int i=s.vrCommands[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
      if(s.vrCommands[0][i].length()>99)  return false;
    }
  }
  if(s.cmdIcon && !ImageMarshaller::checkIntegrityConst(*s.cmdIcon))  return false;
  return true;
}

Json::Value AddCommand_requestMarshaller::toJSON(const AddCommand_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["cmdID"]=Json::Value(e.cmdID);

  if(e.menuParams)
    json["menuParams"]=MenuParamsMarshaller::toJSON(*e.menuParams);

  if(e.vrCommands)
  {
    unsigned int sz=e.vrCommands->size();
    json["vrCommands"]=Json::Value(Json::arrayValue);
    json["vrCommands"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["vrCommands"][i]=Json::Value(e.vrCommands[0][i]);
  }

  if(e.cmdIcon)
    json["cmdIcon"]=ImageMarshaller::toJSON(*e.cmdIcon);

  return json;
}


bool AddCommand_requestMarshaller::fromJSON(const Json::Value& json,AddCommand_request& c)
{
  if(c.menuParams)  delete c.menuParams;
  c.menuParams=0;

  if(c.vrCommands)  delete c.vrCommands;
  c.vrCommands=0;

  if(c.cmdIcon)  delete c.cmdIcon;
  c.cmdIcon=0;

  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("cmdID"))  return false;
    {
      const Json::Value& j=json["cmdID"];
      if(!j.isInt())  return false;
      c.cmdID=j.asInt();
    }
    if(json.isMember("menuParams"))
    {
      const Json::Value& j=json["menuParams"];
      c.menuParams=new MenuParams();
      if(!MenuParamsMarshaller::fromJSON(j,c.menuParams[0]))
        return false;
    }
    if(json.isMember("vrCommands"))
    {
      const Json::Value& j=json["vrCommands"];
      if(!j.isArray())  return false;
      c.vrCommands=new std::vector<std::string>();
      c.vrCommands->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        if(!j[i].isString())
          return false;
        else
          c.vrCommands[0][i]=j[i].asString();
    }
    if(json.isMember("cmdIcon"))
    {
      const Json::Value& j=json["cmdIcon"];
      c.cmdIcon=new Image();
      if(!ImageMarshaller::fromJSON(j,c.cmdIcon[0]))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

