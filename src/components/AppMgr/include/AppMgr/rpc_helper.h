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

#ifndef SRC_COMPONENTS_APPMGR_INCLUDE_APPMGR_RPC_HELPER_H_
#define SRC_COMPONENTS_APPMGR_INCLUDE_APPMGR_RPC_HELPER_H_

#include <stddef.h>
#include <vector>

#include "AppMgr/Application_v2.h"
#include "AppMgr/HMIHandler.h"
#include "AppMgr/MobileHandler.h"
#include "AppMgr/AppMgrCore.h"

#include "JSONHandler/SDLRPCMessage.h"
#include "JSONHandler/SDLRPCObjects/V2/HMILevel.h"
#include "JSONHandler/SDLRPCObjects/V2/Result.h"
#include "JSONHandler/SDLRPCObjects/V2/FunctionID.h"

namespace rpc_commands {
namespace helpers {

typedef NsAppManager::Application_v2 App_V2;

template<typename To, typename From, typename InternalType>
void convert(const std::vector<From> & from, std::vector<To> & result) {
  typename std::vector<From>::const_iterator i = from.begin();
  for (; i != from.end(); ++i) {
    const From& caps = *i;
    To item;
    item.set(static_cast<InternalType>(caps.get()));
    result.push_back(item);
  }
}

template<typename Response>
void sendResponse(int responseId,
                  NsSmartDeviceLinkRPCV2::Result::ResultInternal result) {
  Response* response = new Response;
  if (!response)
    return;

  response->setId(responseId);
  response->setResult(result);
  NsAppManager::HMIHandler::getInstance().sendResponse(response);
}

template<typename Response, typename Result>
void sendResponse(
    NsSmartDeviceLinkRPCV2::FunctionID::FunctionIDInternal functionId,
    Result result, unsigned int correlationID, bool succes,
    unsigned int sessionKey) {
  Response* response = new Response;
  if (!response)
    return;

  response->setMethodId(functionId);
  response->setMessageType(NsSmartDeviceLinkRPC::SDLRPCMessage::RESPONSE);
  response->set_success(succes);
  response->set_resultCode(result);
  response->setCorrelationID(correlationID);
  NsAppManager::MobileHandler::getInstance().sendRPCMessage(response,
                                                            sessionKey);
}

template<typename Response, typename Result>
void sendResponse(NsSmartDeviceLinkRPC::SDLRPCMessage * request, Result result,
                  bool succes, unsigned int sessionKey) {
  Response* response = new Response;
  if (!response)
    return;

  response->setMethodId(request->getMethodId());
  response->setMessageType(NsSmartDeviceLinkRPC::SDLRPCMessage::RESPONSE);
  response->setCorrelationID(request->getCorrelationID());
  response->set_success(succes);
  response->set_resultCode(result);
  NsAppManager::MobileHandler::getInstance().sendRPCMessage(response,
                                                            sessionKey);
}

template<typename Response>
App_V2* getApplicationV2AndCheckHMIStatus(
    unsigned int sessionKey, NsSmartDeviceLinkRPC::SDLRPCMessage * request) {
  App_V2* app =
      static_cast<App_V2*>(NsAppManager::AppMgrCore::getInstance()
          .getItem(sessionKey));
  if (!app) {
    /*LOG4CXX_ERROR_EXT(mLogger, " session key " << sessionKey
     << " hasn't been associated with any application!");*/

    sendResponse<Response, NsSmartDeviceLinkRPCV2::Result::ResultInternal>(
        request, NsSmartDeviceLinkRPCV2::Result::APPLICATION_NOT_REGISTERED,
        false, sessionKey);

    return NULL;
  }

  if (NsSmartDeviceLinkRPCV2::HMILevel::HMI_NONE
      == app->getApplicationHMIStatusLevel()) {
    // LOG4CXX_ERROR_EXT(mLogger, "An application " << app->getName()
    //                     << " with session key " << sessionKey

    sendResponse<Response, NsSmartDeviceLinkRPCV2::Result::ResultInternal>(
        request, NsSmartDeviceLinkRPCV2::Result::REJECTED, false, sessionKey);

    return NULL;
  }

  return app;
}
}  // namespace helpers
}  // namespace rpc_commands

#endif  // SRC_COMPONENTS_APPMGR_INCLUDE_APPMGR_RPC_HELPER_H_
