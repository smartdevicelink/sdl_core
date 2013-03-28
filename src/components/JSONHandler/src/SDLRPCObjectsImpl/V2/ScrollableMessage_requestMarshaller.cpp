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

#include "../include/JSONHandler/SDLRPCObjects/V2/ScrollableMessage_request.h"
#include "SoftButtonMarshaller.h"

#include "ScrollableMessage_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool ScrollableMessage_requestMarshaller::checkIntegrity(ScrollableMessage_request& s)
{
  return checkIntegrityConst(s);
}


bool ScrollableMessage_requestMarshaller::fromString(const std::string& s,ScrollableMessage_request& e)
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


const std::string ScrollableMessage_requestMarshaller::toString(const ScrollableMessage_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ScrollableMessage_requestMarshaller::checkIntegrityConst(const ScrollableMessage_request& s)
{
  if(s.scrollableMessageBody.length()>500)  return false;
  if(s.timeout && *s.timeout>65535)  return false;
  if(s.softButtons)
  {
    unsigned int i=s.softButtons[0].size();
    if(i>8 || i<0)  return false;
    while(i--)
    {
    if(!SoftButtonMarshaller::checkIntegrityConst(s.softButtons[0][i]))   return false;
    }
  }
  return true;
}

Json::Value ScrollableMessage_requestMarshaller::toJSON(const ScrollableMessage_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["scrollableMessageBody"]=Json::Value(e.scrollableMessageBody);

  if(e.timeout)
    json["timeout"]=Json::Value(*e.timeout);

  if(e.softButtons)
  {
    unsigned int sz=e.softButtons->size();
    json["softButtons"]=Json::Value(Json::arrayValue);
    json["softButtons"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["softButtons"][i]=SoftButtonMarshaller::toJSON(e.softButtons[0][i]);
  }

  return json;
}


bool ScrollableMessage_requestMarshaller::fromJSON(const Json::Value& json,ScrollableMessage_request& c)
{
  if(c.timeout)  delete c.timeout;
  c.timeout=0;

  if(c.softButtons)  delete c.softButtons;
  c.softButtons=0;

  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("scrollableMessageBody"))  return false;
    {
      const Json::Value& j=json["scrollableMessageBody"];
      if(!j.isString())  return false;
      c.scrollableMessageBody=j.asString();
    }
    if(json.isMember("timeout"))
    {
      const Json::Value& j=json["timeout"];
      if(!j.isInt())  return false;
      c.timeout=new unsigned int(j.asInt());
    }
    if(json.isMember("softButtons"))
    {
      const Json::Value& j=json["softButtons"];
      if(!j.isArray())  return false;
      c.softButtons=new std::vector<SoftButton>();
      c.softButtons->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        SoftButton t;
        if(!SoftButtonMarshaller::fromJSON(j[i],t))
          return false;
        c.softButtons[0][i]=t;
      }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

