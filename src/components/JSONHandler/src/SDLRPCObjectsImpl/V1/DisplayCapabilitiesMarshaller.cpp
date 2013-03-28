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

#include "../include/JSONHandler/SDLRPCObjects/V1/DisplayCapabilities.h"
#include "DisplayTypeMarshaller.h"
#include "MediaClockFormatMarshaller.h"
#include "TextFieldMarshaller.h"

#include "DisplayCapabilitiesMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPC;


bool DisplayCapabilitiesMarshaller::checkIntegrity(DisplayCapabilities& s)
{
  return checkIntegrityConst(s);
}


bool DisplayCapabilitiesMarshaller::fromString(const std::string& s,DisplayCapabilities& e)
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


const std::string DisplayCapabilitiesMarshaller::toString(const DisplayCapabilities& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool DisplayCapabilitiesMarshaller::checkIntegrityConst(const DisplayCapabilities& s)
{
  if(!DisplayTypeMarshaller::checkIntegrityConst(s.displayType))  return false;
  {
    unsigned int i=s.mediaClockFormats.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!MediaClockFormatMarshaller::checkIntegrityConst(s.mediaClockFormats[i]))   return false;
    }
  }
  {
    unsigned int i=s.textFields.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TextFieldMarshaller::checkIntegrityConst(s.textFields[i]))   return false;
    }
  }
  return true;
}

Json::Value DisplayCapabilitiesMarshaller::toJSON(const DisplayCapabilities& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["displayType"]=DisplayTypeMarshaller::toJSON(e.displayType);

  json["mediaClockFormats"]=Json::Value(Json::arrayValue);
  json["mediaClockFormats"].resize(e.mediaClockFormats.size());
  for(unsigned int i=0;i<e.mediaClockFormats.size();i++)
    json["mediaClockFormats"][i]=MediaClockFormatMarshaller::toJSON(e.mediaClockFormats[i]);

  json["textFields"]=Json::Value(Json::arrayValue);
  json["textFields"].resize(e.textFields.size());
  for(unsigned int i=0;i<e.textFields.size();i++)
    json["textFields"][i]=TextFieldMarshaller::toJSON(e.textFields[i]);


  return json;
}


bool DisplayCapabilitiesMarshaller::fromJSON(const Json::Value& json,DisplayCapabilities& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("displayType"))  return false;
    {
      const Json::Value& j=json["displayType"];
      if(!DisplayTypeMarshaller::fromJSON(j,c.displayType))
        return false;
    }
    if(!json.isMember("mediaClockFormats"))  return false;
    {
      const Json::Value& j=json["mediaClockFormats"];
      if(!j.isArray())  return false;
      c.mediaClockFormats.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          MediaClockFormat t;
          if(!MediaClockFormatMarshaller::fromJSON(j[i],t))
            return false;
          c.mediaClockFormats[i]=t;
        }

    }
    if(!json.isMember("textFields"))  return false;
    {
      const Json::Value& j=json["textFields"];
      if(!j.isArray())  return false;
      c.textFields.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          TextField t;
          if(!TextFieldMarshaller::fromJSON(j[i],t))
            return false;
          c.textFields[i]=t;
        }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

