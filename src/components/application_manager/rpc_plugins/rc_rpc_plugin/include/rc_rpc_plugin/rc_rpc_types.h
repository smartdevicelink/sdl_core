/*
 * Copyright (c) 2019, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_TYPES_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_TYPES_H

#include <ctime>
#include <string>
#include <utility>
#include <vector>

namespace resumption {
class LastState;
}  // namespace resumption

namespace application_manager {
class ApplicationManager;
}  // namespace application_manager

namespace rc_rpc_plugin {
namespace rc_rpc_types {
/**
 * @brief Module resource (module_type(first) + module_id(second))
 */
typedef std::pair<std::string, std::string> ModuleUid;

/**
 * @brief Module consent enum.
 */
enum class ModuleConsent {
  NOT_EXISTS = 0, /**< Consent is absent for specified module resource */
  CONSENTED,      /**< Module resource is consented */
  NOT_CONSENTED   /**< Module resource isn't consented */
};

enum class ModuleConsentState {
  NOT_EXISTS = 0, /**< Consent is absent for specified module resource */
  EXPIRED,        /**< Module resource is expired */
  ACTIVE          /**< Module resource isn't expired */
};

/**
 * @brief Module consent
 * @param module_id - Module resource for consent
 * @param consent - Is module resource consented by driver
 * @param date_of_consent - Date, when module resource has been consented (in
 * seconds [UNIX time])
 */
struct ModuleIdConsent {
  ModuleUid module_id;
  ModuleConsent consent;
  std::time_t date_of_consent;
};

/**
 * @brief ModuleIdConsent collection
 */
typedef std::vector<ModuleIdConsent> ModuleIdConsentVector;
}  // namespace rc_rpc_types
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_TYPES_H
