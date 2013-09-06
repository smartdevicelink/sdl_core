/**
* Copyright (c) 2013, Ford Motor Company
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following
* disclaimer in the documentation and/or other materials provided with the
* distribution.
*
* Neither the name of the Ford Motor Company nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#include "hmi_message_handler/dbus_message_controller.h"
#include "CMessageBroker.hpp"

using NsMessageBroker::ErrorCode;

namespace hmi_message_handler {

DBusMessageController::DBusMessageController(const std::string& serviceName,
                                             const std::string& path)
    : DBusAdapter(serviceName, path),
      mControllersIdStart(-1) {}

void DBusMessageController::subscribeTo(std::string property) {
}

void DBusMessageController::SendJsonMessage(const Json::Value& message) {
  std::string mes = m_writer.write(message);
  Send(mes);
  if (!isNotification(message) && !isResponse(message)) {
    // not notification, not a response, store id and method name to recognize an answer
    mWaitResponseQueue.insert(std::map<std::string, std::string>::value_type(
        message["id"].asString(),
        message["method"].asString()));
  }
}

bool DBusMessageController::isNotification(const Json::Value& root) {
  bool ret = false;
  if (false == root.isMember("id")) {
     ret = true;
  }
  return ret;
}

bool DBusMessageController::isResponse(const Json::Value& root) {
  bool ret = false;
  if ((true == root.isMember("result")) || (true == root.isMember("error"))) {
     ret = true;
  }
  return ret;
}

void DBusMessageController::Send(const std::string& data) {
  DBusAdapter::Send(data);
}

void DBusMessageController::Recv(std::string& data) {
  if (DBusAdapter::Recv(data)) {
    Json::Value root;
    if (m_reader.parse(data, root)) {
      onMessageReceived(root);
    }
  }
}

void DBusMessageController::onMessageReceived(Json::Value message) {
  // Determine message type and process...
  Json::Value error;
  if (checkMessage(message, error))
  {
     if (isNotification(message))
     {
        processNotification(message);
     } else if (isResponse(message))
     {
        std::string id = message["id"].asString();
        std::string method = findMethodById(id);
        if ("" != method)
        {
           if ("MB.registerComponent" == method)
           { // initialize mControllersIdStart
              if (message.isMember("result") && message["result"].isInt())
              {
                 mControllersIdStart = message["result"].asInt();
              } else
              {
                 // TODO(KKolodiy): log it DBG_MSG_ERROR(("Not possible to initialize mControllersIdStart!\n"));
              }
           } else if ("MB.subscribeTo" == method || "MB.unregisterComponent" == method || "MB.unsubscribeFrom" == method)
           {
              //nothing to do for now
           } else
           {
              processResponse(method, message);
           }
        } else
        {
          // TODO(KKolodiy): log it DBG_MSG_ERROR(("Request with id %s has not been found!\n", id.c_str()));
        }
     } else
     {
       // TODO(KKolodiy): log it DBG_MSG(("Message is request!\n"));
        processRequest(message);
     }
  } else
  {
    // TODO(KKolodiy): log it DBG_MSG_ERROR(("Message contains wrong data!\n"));
  }
}

DBusMessageController::~DBusMessageController() {
}

std::string DBusMessageController::findMethodById(std::string id)
{
   std::string res = "";
   std::map <std::string, std::string>::iterator it;
   it = mWaitResponseQueue.find(id);
   if (it != mWaitResponseQueue.end())
   {
      res = (*it).second;
      mWaitResponseQueue.erase(it);
   }
   return res;
}

bool DBusMessageController::checkMessage(Json::Value& root, Json::Value& error) {
  Json::Value err;

  try
  {
     /* check the JSON-RPC version => 2.0 */
     if (!root.isObject() || !root.isMember("jsonrpc") || root["jsonrpc"] != "2.0")
     {
        error["id"] = Json::Value::null;
        error["jsonrpc"] = "2.0";
        err["code"] = NsMessageBroker::INVALID_REQUEST;
        err["message"] = "Invalid MessageBroker request.";
        error["error"] = err;
        return false;
     }

     if (root.isMember("id") && (root["id"].isArray() || root["id"].isObject()))
     {
        error["id"] = Json::Value::null;
        error["jsonrpc"] = "2.0";
        err["code"] = NsMessageBroker::INVALID_REQUEST;
        err["message"] = "Invalid MessageBroker request.";
        error["error"] = err;
        return false;
     }

     if (root.isMember("method"))
     {
        if (!root["method"].isString())
        {
           error["id"] = Json::Value::null;
           error["jsonrpc"] = "2.0";
           err["code"] = NsMessageBroker::INVALID_REQUEST;
           err["message"] = "Invalid MessageBroker request.";
           error["error"] = err;
           return false;
        }
        /* Check the params is  an object*/
        if (root.isMember("params") && !root["params"].isObject())
        {
           error["id"] = Json::Value::null;
           error["jsonrpc"] = "2.0";
           err["code"] = NsMessageBroker::INVALID_REQUEST;
           err["message"] = "Invalid JSONRPC params.";
           error["error"] = err;
           return false;
        }
     } else if (!root.isMember("result") && !root.isMember("error"))
     {
        return false;
     }
     return true;
  } catch (...)
  {
    // TODO(KKolodiy): log it DBG_MSG_ERROR(("CMessageBrokerController::checkMessage() EXCEPTION has been caught!\n"));
     return false;
  }
}

void* DBusMessageController::MethodForReceiverThread(void*)
{
   while(true)
   {
      std::string data = "";
      Recv(data);
   }
   return NULL;
}

}  // namespace hmi_message_handler

