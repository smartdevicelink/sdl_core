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

#include "../include/JSONHandler/SDLRPCObjects/V2/HMIPermissions.h"
#include "HMILevelMarshaller.h"

#include "HMIPermissionsMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool HMIPermissionsMarshaller::checkIntegrity(HMIPermissions& s)
{
  return checkIntegrityConst(s);
}


bool HMIPermissionsMarshaller::fromString(const std::string& s,HMIPermissions& e)
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


const std::string HMIPermissionsMarshaller::toString(const HMIPermissions& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool HMIPermissionsMarshaller::checkIntegrityConst(const HMIPermissions& s)
{
  {
    unsigned int i=s.allowed.size();
    if(i>100 || i<0)  return false;
    while(i--)
    {
    if(!HMILevelMarshaller::checkIntegrityConst(s.allowed[i]))   return false;
    }
  }
  {
    unsigned int i=s.userDisallowed.size();
    if(i>100 || i<0)  return false;
    while(i--)
    {
    if(!HMILevelMarshaller::checkIntegrityConst(s.userDisallowed[i]))   return false;
    }
  }
  return true;
}

Json::Value HMIPermissionsMarshaller::toJSON(const HMIPermissions& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["allowed"]=Json::Value(Json::arrayValue);
  json["allowed"].resize(e.allowed.size());
  for(unsigned int i=0;i<e.allowed.size();i++)
    json["allowed"][i]=HMILevelMarshaller::toJSON(e.allowed[i]);

  json["userDisallowed"]=Json::Value(Json::arrayValue);
  json["userDisallowed"].resize(e.userDisallowed.size());
  for(unsigned int i=0;i<e.userDisallowed.size();i++)
    json["userDisallowed"][i]=HMILevelMarshaller::toJSON(e.userDisallowed[i]);


  return json;
}


bool HMIPermissionsMarshaller::fromJSON(const Json::Value& json,HMIPermissions& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("allowed"))  return false;
    {
      const Json::Value& j=json["allowed"];
      if(!j.isArray())  return false;
      c.allowed.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          HMILevel t;
          if(!HMILevelMarshaller::fromJSON(j[i],t))
            return false;
          c.allowed[i]=t;
        }

    }
    if(!json.isMember("userDisallowed"))  return false;
    {
      const Json::Value& j=json["userDisallowed"];
      if(!j.isArray())  return false;
      c.userDisallowed.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          HMILevel t;
          if(!HMILevelMarshaller::fromJSON(j[i],t))
            return false;
          c.userDisallowed[i]=t;
        }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

