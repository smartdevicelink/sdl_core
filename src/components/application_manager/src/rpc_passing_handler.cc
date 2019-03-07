/*
 * Copyright (c) 2018, Ford Motor Company
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

#include "application_manager/help_prompt_manager_impl.h"

#include <algorithm>
#include <iterator>

#include "application_manager/application.h"
#include "application_manager/application_manager.h"
#include "application_manager/rpc_passing_handler.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "encryption/hashing.h"
#include "resumption/last_state.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/logger.h"

#include "smart_objects/enum_schema_item.h"
#include "utils/timer_task_impl.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "AppServiceManager")

namespace application_manager {

RPCPassingHandler::RPCPassingHandler(AppServiceManager& asm_ref,
                                     ApplicationManager& am_ref)
    : app_service_manager_(asm_ref), app_manager_(am_ref) {}

bool RPCPassingHandler::UsingAppServices(uint32_t correlation_id) {
  if (messages_using_core_.find(correlation_id) == messages_using_core_.end()) {
    return true;
  }
  messages_using_core_.erase(correlation_id);
  return false;
}

bool RPCPassingHandler::CanUseRPCPassing(int32_t function_id) {
  auto services = app_service_manager_.GetActiveServices();
  for (auto it = services.begin(); it != services.end(); ++it) {
    auto handled_rpcs =
        it->record[strings::service_manifest][strings::handled_rpcs];
    for (size_t i = 0; i < handled_rpcs.length(); i++) {
      if (handled_rpcs[i].asInt() == function_id) {
        return true;
      }
    }
  }
  return false;
}

bool RPCPassingHandler::RPCPassThrough(smart_objects::SmartObject rpc_message) {
  uint32_t origin_connection_key, correlation_id;
  int32_t function_id, message_type;

  origin_connection_key =
      rpc_message[strings::params][strings::connection_key].asUInt();
  correlation_id =
      rpc_message[strings::params][strings::correlation_id].asUInt();
  function_id = rpc_message[strings::params][strings::function_id].asInt();
  message_type = rpc_message[strings::params][strings::message_type].asInt();

  LOG4CXX_DEBUG(logger_, "RPC_PASSING: ");
  LOG4CXX_DEBUG(logger_, "origin_connection_key: " << origin_connection_key);
  LOG4CXX_DEBUG(logger_, "correlation_id: " << correlation_id);
  LOG4CXX_DEBUG(logger_, "function_id: " << function_id);
  LOG4CXX_DEBUG(logger_, "message_type: " << message_type);

  switch (message_type) {
    case MessageType::kRequest: {
      LOG4CXX_DEBUG(logger_, "Handle request");

      auto it = request_queue_.find(correlation_id);
      // If correlation_id not in pass_through_requests
      if (it == request_queue_.end()) {
        LOG4CXX_DEBUG(logger_,
                      "Correlation id does NOT exist in map. Constructing "
                      "PassThroughRequests");
        // Construct list of pass through services
        request_queue_[correlation_id] = std::queue<RpcPassThroughRequest>();
        auto services = app_service_manager_.GetActiveServices();

        for (auto services_it = services.begin(); services_it != services.end();
             ++services_it) {
          auto handled_rpcs =
              services_it
                  ->record[strings::service_manifest][strings::handled_rpcs];
          for (size_t i = 0; i < handled_rpcs.length(); i++) {
            if (handled_rpcs[i].asInt() == function_id) {
              // Add Passthrough requests to queue
              rpc_message[strings::params][strings::connection_key] =
                  services_it->connection_key;
              auto req = RpcPassThroughRequest{origin_connection_key,
                                               services_it->connection_key,
                                               rpc_message};
              request_queue_[correlation_id].push(req);
              app_manager_.IncreaseForwardedRequestTimeout(
                  origin_connection_key, correlation_id);
              break;
            }
          }
        }

        rpc_message[strings::params][strings::connection_key] =
            origin_connection_key;
        auto req = RpcPassThroughRequest{
            origin_connection_key, origin_connection_key, rpc_message};
        request_queue_[correlation_id].push(req);

        LOG4CXX_DEBUG(logger_,
                      "Added " << request_queue_[correlation_id].size()
                               << " requests to the queue");
        CycleThroughRPCPassingRequests(correlation_id);

      } else {
        LOG4CXX_DEBUG(logger_, "Correlation id does exist in map. Continuing");
        return false;
      }

      if (request_queue_[correlation_id].empty()) {
        LOG4CXX_DEBUG(logger_,
                      "No services left in map. using core to handle request");
        request_queue_.erase(correlation_id);
        current_request_.erase(correlation_id);
        messages_using_core_.insert(correlation_id);
        return false;
      } else {
        LOG4CXX_DEBUG(
            logger_,
            "Forwarding request to service with app id "
                << current_request_[correlation_id].destination_connection_key);
        smart_objects::SmartObjectSPtr result =
            std::make_shared<smart_objects::SmartObject>(
                current_request_[correlation_id].message);
        AddTimeoutTimer(correlation_id);
        app_manager_.GetRPCService().SendMessageToMobile(result);
        return true;
      }

      break;
    }

    case MessageType::kResponse: {
      LOG4CXX_DEBUG(logger_, "Handle response");
      if (!rpc_message[strings::msg_params].keyExists(strings::result_code)) {
        break;
      }
      auto result_code = static_cast<mobile_apis::Result::eType>(
          rpc_message[strings::msg_params][strings::result_code].asInt());
      if (result_code == mobile_apis::Result::UNSUPPORTED_REQUEST) {
        LOG4CXX_DEBUG(logger_, "Service sent unsupported request");
        CycleThroughRPCPassingRequests(correlation_id);
        if (request_queue_[correlation_id].empty()) {
          LOG4CXX_DEBUG(
              logger_, "No services left in map. using core to handle request");
          current_request_[correlation_id]
              .message[strings::params][strings::connection_key] =
              current_request_[correlation_id].origin_connection_key;
          smart_objects::SmartObjectSPtr result =
              std::make_shared<smart_objects::SmartObject>(
                  current_request_[correlation_id].message);
          request_queue_.erase(correlation_id);
          current_request_.erase(correlation_id);
          messages_using_core_.insert(correlation_id);
          RemoveTimeoutTimer(correlation_id);
          app_manager_.GetRPCService().ManageMobileCommand(
              result, commands::Command::SOURCE_MOBILE);
        } else {
          LOG4CXX_DEBUG(logger_,
                        "Forwarding request to service with app id "
                            << current_request_[correlation_id]
                                   .destination_connection_key);
          smart_objects::SmartObjectSPtr result =
              std::make_shared<smart_objects::SmartObject>(
                  current_request_[correlation_id].message);
          AddTimeoutTimer(correlation_id);
          app_manager_.GetRPCService().SendMessageToMobile(result);
        }
        return true;
      }

      LOG4CXX_DEBUG(logger_, "Valid RPC passing response");

      if (current_request_.find(correlation_id) != current_request_.end()) {
        LOG4CXX_DEBUG(
            logger_,
            "Forwarding response to mobile app "
                << current_request_[correlation_id].origin_connection_key);
        rpc_message[strings::params][strings::connection_key] =
            current_request_[correlation_id].origin_connection_key;
        smart_objects::SmartObjectSPtr result =
            std::make_shared<smart_objects::SmartObject>(rpc_message);

        request_queue_.erase(correlation_id);
        current_request_.erase(correlation_id);
        RemoveTimeoutTimer(correlation_id);
        app_manager_.GetRPCService().SendMessageToMobile(result);
        return true;
      }

      return false;
      break;
    }
  }

  return false;
}

bool RPCPassingHandler::CycleThroughRPCPassingRequests(
    uint32_t correlation_id) {
  LOG4CXX_DEBUG(logger_, "Cycling through pass through requests");
  if (request_queue_.find(correlation_id) == request_queue_.end()) {
    LOG4CXX_ERROR(logger_, "Correlation id does NOT exist in map");
    return false;
  }

  current_request_[correlation_id] = request_queue_[correlation_id].front();
  request_queue_[correlation_id].pop();

  MessageHelper::PrintSmartObject(current_request_[correlation_id].message);
  return true;
}

void RPCPassingHandler::OnPassThroughRequestTimeout() {
  LOG4CXX_DEBUG(logger_, "RPC Passing request timeout");
  auto timeout_entry = timeout_queue_.front();
  uint32_t correlation_id = timeout_entry.second;
  RemoveTimeoutTimer(correlation_id);
  LOG4CXX_DEBUG(logger_,
                "Cycling through requests for correlation_id "
                    << correlation_id);
  CycleThroughRPCPassingRequests(correlation_id);
  if (request_queue_[correlation_id].empty()) {
    LOG4CXX_DEBUG(logger_,
                  "No services left in map. using core to handle request");
    current_request_[correlation_id]
        .message[strings::params][strings::connection_key] =
        current_request_[correlation_id].origin_connection_key;
    smart_objects::SmartObjectSPtr result =
        std::make_shared<smart_objects::SmartObject>(
            current_request_[correlation_id].message);
    request_queue_.erase(correlation_id);
    current_request_.erase(correlation_id);
    messages_using_core_.insert(correlation_id);
    RemoveTimeoutTimer(correlation_id);
    app_manager_.GetRPCService().ManageMobileCommand(
        result, commands::Command::SOURCE_MOBILE);
  } else {
    LOG4CXX_DEBUG(
        logger_,
        "Forwarding request to service with app id "
            << current_request_[correlation_id].destination_connection_key);
    smart_objects::SmartObjectSPtr result =
        std::make_shared<smart_objects::SmartObject>(
            current_request_[correlation_id].message);
    AddTimeoutTimer(correlation_id);
    app_manager_.GetRPCService().SendMessageToMobile(result);
  }
}

void RPCPassingHandler::AddTimeoutTimer(uint32_t correlation_id) {
  TimerSPtr rpc_passing_timer(std::make_shared<timer::Timer>(
      "RPCPassingTimeoutTimer_" + std::to_string(correlation_id),
      new timer::TimerTaskImpl<RPCPassingHandler>(
          this, &RPCPassingHandler::OnPassThroughRequestTimeout)));
  const uint32_t timeout_ms =
      app_manager_.get_settings().rpc_pass_through_timeout();
  rpc_passing_timer->Start(timeout_ms, timer::kSingleShot);
  LOG4CXX_DEBUG(logger_,
                "Adding and starting timer for correlation id "
                    << correlation_id);
  timeout_queue_.push_back(std::make_pair(rpc_passing_timer, correlation_id));
}

void RPCPassingHandler::RemoveTimeoutTimer(uint32_t correlation_id) {
  std::vector<std::vector<std::pair<TimerSPtr, uint32_t> >::iterator>
      timers_to_remove;

  for (auto it = timeout_queue_.begin(); it != timeout_queue_.end(); ++it) {
    uint32_t cid = it->second;
    if (cid == correlation_id) {
      timers_to_remove.push_back(it);
    }
  }

  for (auto it = timers_to_remove.begin(); it != timers_to_remove.end(); ++it) {
    LOG4CXX_DEBUG(logger_,
                  "Removing RPCPassing timer for correlation id "
                      << correlation_id);
    TimerSPtr timer = (*it)->first;
    timer->Stop();
    timeout_queue_.erase(*it);
  }
}

}  //  namespace application_manager
