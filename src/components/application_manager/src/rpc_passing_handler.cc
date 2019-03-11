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
#include "application_manager/rpc_handler_impl.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "encryption/hashing.h"
#include "resumption/last_state.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/logger.h"

#include "smart_objects/enum_schema_item.h"
#include "utils/timer_task_impl.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "RPCPassingHandler")

namespace application_manager {

RPCPassingHandler::RPCPassingHandler(AppServiceManager& asm_ref,
                                     ApplicationManager& am_ref)
    : app_service_manager_(asm_ref), app_manager_(am_ref) {}

RPCPassingHandler::~RPCPassingHandler() {}

bool RPCPassingHandler::IsPassThroughMessage(
    uint32_t correlation_id,
    commands::Command::CommandSource source,
    int32_t message_type) {
  sync_primitives::AutoLock lock(rpc_request_queue_lock_);

  if (rpc_request_queue.find(correlation_id) != rpc_request_queue.end()) {
    if (message_type == MessageType::kResponse &&
        source == commands::Command::CommandSource::SOURCE_SDL) {
      // Checks if response is being sent to mobile then removes the correlation
      // id from the map
      rpc_request_queue.erase(correlation_id);
    }
    return true;
  }
  return false;
}

bool RPCPassingHandler::CanHandleFunctionID(int32_t function_id) {
  auto services = app_service_manager_.GetActiveServices();
  for (auto it = services.begin(); it != services.end(); ++it) {
    auto handled_rpcs =
        it->second.record[strings::service_manifest][strings::handled_rpcs];
    for (size_t i = 0; i < handled_rpcs.length(); i++) {
      if (handled_rpcs[i].asInt() == function_id) {
        return true;
      }
    }
  }
  return false;
}

bool RPCPassingHandler::RPCPassThrough(smart_objects::SmartObject rpc_message) {
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t correlation_id =
      rpc_message[strings::params][strings::correlation_id].asUInt();
  int32_t message_type =
      rpc_message[strings::params][strings::message_type].asInt();

  LOG4CXX_DEBUG(logger_, "RPC_PASSING: ");
  MessageHelper::PrintSmartObject(rpc_message);

  // Clear timers for timed out requests
  ClearCompletedTimers();
  switch (message_type) {
    case MessageType::kRequest: {
      LOG4CXX_DEBUG(logger_, "Handle request");
      rpc_request_queue_lock_.Acquire();
      if (rpc_request_queue.find(correlation_id) == rpc_request_queue.end()) {
        rpc_request_queue_lock_.Release();
        LOG4CXX_DEBUG(
            logger_,
            "Correlation id DOES NOT exist in map. Constructing request queue");
        PopulateRPCRequestQueue(rpc_message);
      } else {
        rpc_request_queue_lock_.Release();
        LOG4CXX_DEBUG(logger_, "Correlation id DOES exist in map. Continuing");
        return false;
      }

      rpc_request_queue_lock_.Acquire();
      if (rpc_request_queue[correlation_id].second.empty()) {
        LOG4CXX_DEBUG(logger_,
                      "No services left in map. Using core to handle request");
        rpc_request_queue.erase(correlation_id);
        rpc_request_queue_lock_.Release();
        return false;
      } else {
        rpc_request_queue_lock_.Release();
        ForwardRequestToMobile(correlation_id);
        return true;
      }

      break;
    }

    case MessageType::kResponse: {
      LOG4CXX_DEBUG(logger_, "Handle response");
      rpc_request_queue_lock_.Acquire();
      if (rpc_request_queue.find(correlation_id) == rpc_request_queue.end()) {
        rpc_request_queue_lock_.Release();
        return false;
      }
      rpc_request_queue_lock_.Release();
      RemoveRequestTimer(correlation_id);

      mobile_apis::Result::eType result_code;
      smart_objects::EnumConversionHelper<mobile_apis::Result::eType>::
          StringToEnum(
              rpc_message[strings::msg_params][strings::result_code].asString(),
              &result_code);

      if (result_code == mobile_apis::Result::UNSUPPORTED_REQUEST) {
        LOG4CXX_DEBUG(logger_, "Service sent UNSUPPORTED_REQUEST");
        PerformNextRequest(correlation_id);
        return true;
      } else {
        LOG4CXX_DEBUG(logger_, "Valid RPC passing response");
        ForwardResponseToMobile(correlation_id, rpc_message);
        return true;
      }

      break;
    }
  }

  return false;
}

void RPCPassingHandler::PopulateRPCRequestQueue(
    smart_objects::SmartObject request_message) {
  uint32_t origin_connection_key =
      request_message[strings::params][strings::connection_key].asUInt();
  uint32_t correlation_id =
      request_message[strings::params][strings::correlation_id].asUInt();
  int32_t function_id =
      request_message[strings::params][strings::function_id].asInt();

  // Construct list of pass through services
  auto entry = std::make_pair(request_message, std::deque<ServiceInfo>());
  auto services = app_service_manager_.GetActiveServices();

  for (auto services_it = services.begin(); services_it != services.end();
       ++services_it) {
    auto handled_rpcs =
        services_it->second
            .record[strings::service_manifest][strings::handled_rpcs];
    for (size_t i = 0; i < handled_rpcs.length(); i++) {
      if (handled_rpcs[i].asInt() == function_id) {
        // Add requests to queue
        ServiceInfo service_info{services_it->first,
                                 services_it->second.connection_key};
        entry.second.push_back(service_info);
        app_manager_.IncreaseForwardedRequestTimeout(origin_connection_key,
                                                     correlation_id);
        break;
      }
    }
  }

  sync_primitives::AutoLock lock(rpc_request_queue_lock_);
  rpc_request_queue[correlation_id] = entry;

  LOG4CXX_DEBUG(logger_,
                "Added " << rpc_request_queue[correlation_id].second.size()
                         << " requests to the queue");
}
void RPCPassingHandler::ForwardRequestToMobile(uint32_t correlation_id) {
  rpc_request_queue_lock_.Acquire();
  uint32_t connection_key =
      rpc_request_queue[correlation_id].second.front().connection_key;
  LOG4CXX_DEBUG(logger_,
                "Forwarding request to service with app id " << connection_key);

  smart_objects::SmartObject message(rpc_request_queue[correlation_id].first);
  message[strings::params][strings::connection_key] = connection_key;
  smart_objects::SmartObjectSPtr result =
      std::make_shared<smart_objects::SmartObject>(message);
  rpc_request_queue_lock_.Release();
  AddRequestTimer(correlation_id);
  app_manager_.GetRPCService().SendMessageToMobile(result);
}

void RPCPassingHandler::ForwardRequestToCore(uint32_t correlation_id) {
  LOG4CXX_DEBUG(logger_,
                "No services left in map. using core to handle request");
  rpc_request_queue_lock_.Acquire();
  smart_objects::SmartObject message(rpc_request_queue[correlation_id].first);
  smart_objects::SmartObjectSPtr result =
      std::make_shared<smart_objects::SmartObject>(message);
  rpc_request_queue.erase(correlation_id);
  rpc_request_queue_lock_.Release();

  // Revalidate rpc message before forwarding to core
  uint32_t connection_key =
      message[strings::params][strings::connection_key].asUInt();
  int32_t function_id = message[strings::params][strings::function_id].asInt();

  auto app_ptr = app_manager_.application(connection_key);
  utils::SemanticVersion msg_version(0, 0, 0);
  if (app_ptr) {
    msg_version = app_ptr->msg_version();
  }
  auto rpc_handler_impl =
      dynamic_cast<rpc_handler::RPCHandlerImpl*>(&app_manager_.GetRPCHandler());
  rpc_handler_impl->ValidateRPCSO(
      &message, connection_key, correlation_id, function_id, msg_version, true);
  app_manager_.GetRPCService().ManageMobileCommand(
      result, commands::Command::SOURCE_MOBILE);
}

void RPCPassingHandler::ForwardResponseToMobile(
    uint32_t correlation_id, smart_objects::SmartObject response_message) {
  rpc_request_queue_lock_.Acquire();
  uint32_t origin_connection_key =
      rpc_request_queue[correlation_id]
          .first[strings::params][strings::connection_key]
          .asUInt();
  rpc_request_queue_lock_.Release();

  LOG4CXX_DEBUG(logger_,
                "Forwarding response to mobile app " << origin_connection_key);
  smart_objects::SmartObject message(response_message);
  message[strings::params][strings::connection_key] = origin_connection_key;
  smart_objects::SmartObjectSPtr result =
      std::make_shared<smart_objects::SmartObject>(message);
  app_manager_.GetRPCService().SendMessageToMobile(result);
}

bool RPCPassingHandler::PerformNextRequest(uint32_t correlation_id) {
  LOG4CXX_DEBUG(logger_, "Performing next request in queue");
  rpc_request_queue_lock_.Acquire();
  if (rpc_request_queue.find(correlation_id) == rpc_request_queue.end()) {
    LOG4CXX_ERROR(logger_, "Correlation id does NOT exist in map");
    rpc_request_queue_lock_.Release();
    return false;
  }
  rpc_request_queue[correlation_id].second.pop_front();
  bool empty = rpc_request_queue[correlation_id].second.empty();
  rpc_request_queue_lock_.Release();

  if (empty) {
    ForwardRequestToCore(correlation_id);
  } else {
    ForwardRequestToMobile(correlation_id);
  }

  return true;
}

void RPCPassingHandler::OnPassThroughRequestTimeout() {
  timeout_queue_lock_.Acquire();
  LOG4CXX_DEBUG(logger_, "Request Timed out");
  auto timeout_entry = timeout_queue_.front();
  uint32_t correlation_id = timeout_entry.second;
  timeout_queue_lock_.Release();
  PerformNextRequest(correlation_id);
}

void RPCPassingHandler::ClearCompletedTimers() {
  sync_primitives::AutoLock lock(timeout_queue_lock_);

  LOG4CXX_DEBUG(logger_, "Clearing Completed Timers");
  for (auto it = timeout_queue_.begin(); it != timeout_queue_.end();) {
    TimerSPtr timer = it->first;
    uint32_t cid = it->second;
    if (timer->is_completed()) {
      LOG4CXX_DEBUG(logger_,
                    "Removing completed timer for correlation id " << cid);
      it = timeout_queue_.erase(it);
    } else {
      ++it;
    }
  }
}

void RPCPassingHandler::AddRequestTimer(uint32_t correlation_id) {
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
  sync_primitives::AutoLock lock(timeout_queue_lock_);
  timeout_queue_.push_back(std::make_pair(rpc_passing_timer, correlation_id));
}

void RPCPassingHandler::RemoveRequestTimer(uint32_t correlation_id) {
  sync_primitives::AutoLock lock(timeout_queue_lock_);

  for (auto it = timeout_queue_.begin(); it != timeout_queue_.end();) {
    TimerSPtr timer = it->first;
    uint32_t cid = it->second;
    if (cid == correlation_id) {
      LOG4CXX_DEBUG(logger_,
                    "Removing timer for correlation id " << correlation_id);
      it = timeout_queue_.erase(it);
    } else {
      ++it;
    }
  }
}

}  //  namespace application_manager
