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

#include "../include/JSONHandler/SDLRPCObjects/V1/TextField.h"
#include "CharacterSetMarshaller.h"
#include "TextFieldNameMarshaller.h"

#include "TextFieldMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPC;


bool TextFieldMarshaller::checkIntegrity(TextField& s)
{
  return checkIntegrityConst(s);
}


bool TextFieldMarshaller::fromString(const std::string& s,TextField& e)
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


const std::string TextFieldMarshaller::toString(const TextField& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool TextFieldMarshaller::checkIntegrityConst(const TextField& s)
{
  if(!CharacterSetMarshaller::checkIntegrityConst(s.characterSet))  return false;
  if(!TextFieldNameMarshaller::checkIntegrityConst(s.name))  return false;
  if(s.rows>3)  return false;
  if(s.rows<1)  return false;
  if(s.width>40)  return false;
  if(s.width<1)  return false;
  return true;
}

Json::Value TextFieldMarshaller::toJSON(const TextField& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["characterSet"]=CharacterSetMarshaller::toJSON(e.characterSet);

  json["name"]=TextFieldNameMarshaller::toJSON(e.name);

  json["rows"]=Json::Value(e.rows);

  json["width"]=Json::Value(e.width);


  return json;
}


bool TextFieldMarshaller::fromJSON(const Json::Value& json,TextField& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("characterSet"))  return false;
    {
      const Json::Value& j=json["characterSet"];
      if(!CharacterSetMarshaller::fromJSON(j,c.characterSet))
        return false;
    }
    if(!json.isMember("name"))  return false;
    {
      const Json::Value& j=json["name"];
      if(!TextFieldNameMarshaller::fromJSON(j,c.name))
        return false;
    }
    if(!json.isMember("rows"))  return false;
    {
      const Json::Value& j=json["rows"];
      if(!j.isInt())  return false;
      c.rows=j.asInt();
    }
    if(!json.isMember("width"))  return false;
    {
      const Json::Value& j=json["width"];
      if(!j.isInt())  return false;
      c.width=j.asInt();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

