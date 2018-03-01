/*
 Copyright (c) 2017, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "application_manager/commands/command_impl.h"
#include "smart_objects/smart_object.h"

namespace application_manager {
namespace commands {

namespace command_helper {
/**
 * @brief Adds disallowed parameters back to response with appropriate
 * reasons
 * @param permissions Structure with info about disallowed parameters
 * @param response Response message, which should be extended with blocked
 * parameters reasons
 */
void AddDisallowedParameters(const CommandParametersPermissions& permissions,
                             smart_objects::SmartObject& response);

/**
 * @brief Adds disallowed parameters to response info
 * @param permissions Structure with info about disallowed parameters
 * @param response Response message, which info should be extended
 */
void AddDisallowedParametersToInfo(
    const CommandParametersPermissions& permissions,
    smart_objects::SmartObject& response);

/**
 * @brief Remove from current message parameters disallowed by policy table
 * @param permissions Structure with info about parameters which should be
 * disallowed
 * @param removed_permissions Structure with info about parameters which
 * actually was disallowd in current message
 * @param msg_params Message params where disallowed params should be removed
 */
void RemoveDisallowedParameters(
    const CommandParametersPermissions& permissions,
    CommandParametersPermissions& removed_permissions,
    smart_objects::SmartObject& msg_params);

/**
 * @brief Checks if any request param was marked as disallowed by policy
 * @param permissions Structure with info about parameters which actually was
 * disallowd in current message
 * @return true if any param was marked as disallowed
 */
bool HasDisallowedParams(const CommandParametersPermissions& permissions);

}  // namespace CommandHelper

}  // namespace commands
}  // namespace application_manager
