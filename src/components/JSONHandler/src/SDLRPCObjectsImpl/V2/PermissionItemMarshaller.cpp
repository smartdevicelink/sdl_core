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

#include "../include/JSONHandler/SDLRPCObjects/V2/PermissionItem.h"
#include "HMIPermissionsMarshaller.h"
#include "ParameterPermissionsMarshaller.h"

#include "PermissionItemMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool PermissionItemMarshaller::checkIntegrity(PermissionItem& s)
{
  return checkIntegrityConst(s);
}


bool PermissionItemMarshaller::fromString(const std::string& s,PermissionItem& e)
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


const std::string PermissionItemMarshaller::toString(const PermissionItem& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool PermissionItemMarshaller::checkIntegrityConst(const PermissionItem& s)
{
  if(!HMIPermissionsMarshaller::checkIntegrityConst(s.hmiPermissions))  return false;
  if(!ParameterPermissionsMarshaller::checkIntegrityConst(s.parameterPermissions))  return false;
  if(s.rpcName.length()>100)  return false;
  return true;
}

Json::Value PermissionItemMarshaller::toJSON(const PermissionItem& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["hmiPermissions"]=HMIPermissionsMarshaller::toJSON(e.hmiPermissions);

  json["parameterPermissions"]=ParameterPermissionsMarshaller::toJSON(e.parameterPermissions);

  json["rpcName"]=Json::Value(e.rpcName);


  return json;
}


bool PermissionItemMarshaller::fromJSON(const Json::Value& json,PermissionItem& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("hmiPermissions"))  return false;
    {
      const Json::Value& j=json["hmiPermissions"];
      if(!HMIPermissionsMarshaller::fromJSON(j,c.hmiPermissions))
        return false;
    }
    if(!json.isMember("parameterPermissions"))  return false;
    {
      const Json::Value& j=json["parameterPermissions"];
      if(!ParameterPermissionsMarshaller::fromJSON(j,c.parameterPermissions))
        return false;
    }
    if(!json.isMember("rpcName"))  return false;
    {
      const Json::Value& j=json["rpcName"];
      if(!j.isString())  return false;
      c.rpcName=j.asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

