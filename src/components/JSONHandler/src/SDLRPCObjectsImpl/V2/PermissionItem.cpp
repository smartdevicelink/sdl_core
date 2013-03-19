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
#include "PermissionItemMarshaller.h"
#include "HMIPermissionsMarshaller.h"
#include "ParameterPermissionsMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/



using namespace NsSmartDeviceLinkRPCV2;

PermissionItem::PermissionItem(const PermissionItem& c)
{
  *this=c;
}


bool PermissionItem::checkIntegrity(void)
{
  return PermissionItemMarshaller::checkIntegrity(*this);
}


PermissionItem::PermissionItem(void)
{
}



bool PermissionItem::set_hmiPermissions(const HMIPermissions& hmiPermissions_)
{
  if(!HMIPermissionsMarshaller::checkIntegrityConst(hmiPermissions_))   return false;
  hmiPermissions=hmiPermissions_;
  return true;
}

bool PermissionItem::set_parameterPermissions(const ParameterPermissions& parameterPermissions_)
{
  if(!ParameterPermissionsMarshaller::checkIntegrityConst(parameterPermissions_))   return false;
  parameterPermissions=parameterPermissions_;
  return true;
}

bool PermissionItem::set_rpcName(const std::string& rpcName_)
{
  if(rpcName_.length()>100)  return false;
  rpcName=rpcName_;
  return true;
}




const HMIPermissions& PermissionItem::get_hmiPermissions(void) const 
{
  return hmiPermissions;
}


const ParameterPermissions& PermissionItem::get_parameterPermissions(void) const 
{
  return parameterPermissions;
}


const std::string& PermissionItem::get_rpcName(void) const 
{
  return rpcName;
}


