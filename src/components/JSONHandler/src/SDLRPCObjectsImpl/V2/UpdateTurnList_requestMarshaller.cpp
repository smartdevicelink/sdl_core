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

#include "../include/JSONHandler/SDLRPCObjects/V2/UpdateTurnList_request.h"
#include "SoftButtonMarshaller.h"
#include "TurnMarshaller.h"

#include "UpdateTurnList_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool UpdateTurnList_requestMarshaller::checkIntegrity(UpdateTurnList_request& s)
{
  return checkIntegrityConst(s);
}


bool UpdateTurnList_requestMarshaller::fromString(const std::string& s,UpdateTurnList_request& e)
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


const std::string UpdateTurnList_requestMarshaller::toString(const UpdateTurnList_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool UpdateTurnList_requestMarshaller::checkIntegrityConst(const UpdateTurnList_request& s)
{
  {
    unsigned int i=s.turnList.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TurnMarshaller::checkIntegrityConst(s.turnList[i]))   return false;
    }
  }
  {
    unsigned int i=s.softButtons.size();
    if(i>1 || i<0)  return false;
    while(i--)
    {
    if(!SoftButtonMarshaller::checkIntegrityConst(s.softButtons[i]))   return false;
    }
  }
  return true;
}

Json::Value UpdateTurnList_requestMarshaller::toJSON(const UpdateTurnList_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["turnList"]=Json::Value(Json::arrayValue);
  json["turnList"].resize(e.turnList.size());
  for(unsigned int i=0;i<e.turnList.size();i++)
    json["turnList"][i]=TurnMarshaller::toJSON(e.turnList[i]);

  json["softButtons"]=Json::Value(Json::arrayValue);
  json["softButtons"].resize(e.softButtons.size());
  for(unsigned int i=0;i<e.softButtons.size();i++)
    json["softButtons"][i]=SoftButtonMarshaller::toJSON(e.softButtons[i]);

  return json;
}


bool UpdateTurnList_requestMarshaller::fromJSON(const Json::Value& json,UpdateTurnList_request& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("turnList"))  return false;
    {
      const Json::Value& j=json["turnList"];
      if(!j.isArray())  return false;
      c.turnList.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          Turn t;
          if(!TurnMarshaller::fromJSON(j[i],t))
            return false;
          c.turnList[i]=t;
        }

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

