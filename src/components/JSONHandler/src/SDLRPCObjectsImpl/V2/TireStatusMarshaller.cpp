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

#include "../include/JSONHandler/SDLRPCObjects/V2/TireStatus.h"
#include "SingleTireStatusMarshaller.h"
#include "TirePressureTellTaleMarshaller.h"

#include "TireStatusMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool TireStatusMarshaller::checkIntegrity(TireStatus& s)
{
  return checkIntegrityConst(s);
}


bool TireStatusMarshaller::fromString(const std::string& s,TireStatus& e)
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


const std::string TireStatusMarshaller::toString(const TireStatus& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool TireStatusMarshaller::checkIntegrityConst(const TireStatus& s)
{
  if(!SingleTireStatusMarshaller::checkIntegrityConst(s.innerLeftRear))  return false;
  if(!SingleTireStatusMarshaller::checkIntegrityConst(s.innerRightRear))  return false;
  if(!SingleTireStatusMarshaller::checkIntegrityConst(s.leftFront))  return false;
  if(!SingleTireStatusMarshaller::checkIntegrityConst(s.leftRear))  return false;
  if(!TirePressureTellTaleMarshaller::checkIntegrityConst(s.pressureTellTale))  return false;
  if(!SingleTireStatusMarshaller::checkIntegrityConst(s.rightFront))  return false;
  if(!SingleTireStatusMarshaller::checkIntegrityConst(s.rightRear))  return false;
  return true;
}

Json::Value TireStatusMarshaller::toJSON(const TireStatus& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["innerLeftRear"]=SingleTireStatusMarshaller::toJSON(e.innerLeftRear);

  json["innerRightRear"]=SingleTireStatusMarshaller::toJSON(e.innerRightRear);

  json["leftFront"]=SingleTireStatusMarshaller::toJSON(e.leftFront);

  json["leftRear"]=SingleTireStatusMarshaller::toJSON(e.leftRear);

  json["pressureTellTale"]=TirePressureTellTaleMarshaller::toJSON(e.pressureTellTale);

  json["rightFront"]=SingleTireStatusMarshaller::toJSON(e.rightFront);

  json["rightRear"]=SingleTireStatusMarshaller::toJSON(e.rightRear);


  return json;
}


bool TireStatusMarshaller::fromJSON(const Json::Value& json,TireStatus& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("innerLeftRear"))  return false;
    {
      const Json::Value& j=json["innerLeftRear"];
      if(!SingleTireStatusMarshaller::fromJSON(j,c.innerLeftRear))
        return false;
    }
    if(!json.isMember("innerRightRear"))  return false;
    {
      const Json::Value& j=json["innerRightRear"];
      if(!SingleTireStatusMarshaller::fromJSON(j,c.innerRightRear))
        return false;
    }
    if(!json.isMember("leftFront"))  return false;
    {
      const Json::Value& j=json["leftFront"];
      if(!SingleTireStatusMarshaller::fromJSON(j,c.leftFront))
        return false;
    }
    if(!json.isMember("leftRear"))  return false;
    {
      const Json::Value& j=json["leftRear"];
      if(!SingleTireStatusMarshaller::fromJSON(j,c.leftRear))
        return false;
    }
    if(!json.isMember("pressureTellTale"))  return false;
    {
      const Json::Value& j=json["pressureTellTale"];
      if(!TirePressureTellTaleMarshaller::fromJSON(j,c.pressureTellTale))
        return false;
    }
    if(!json.isMember("rightFront"))  return false;
    {
      const Json::Value& j=json["rightFront"];
      if(!SingleTireStatusMarshaller::fromJSON(j,c.rightFront))
        return false;
    }
    if(!json.isMember("rightRear"))  return false;
    {
      const Json::Value& j=json["rightRear"];
      if(!SingleTireStatusMarshaller::fromJSON(j,c.rightRear))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

