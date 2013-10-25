/**
 * @file policy_manager.h
 * @brief Polocy Manager  header file.
 */
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
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_POLICY_MANAGER_H_
#define SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_POLICY_MANAGER_H_

#include "policies/policy_configuration.h"
#include "smart_objects/smart_object.h"
#include "smart_objects/smart_schema.h"

namespace NsSmartDeviceLink {
namespace policies {

 /**
 * @brief enumeration of result for asking permissions by application
 **/
namespace CheckPermissionResult {
enum eType {
  PERMISSION_OK = 0,
  PERMISSION_NOK_PT_VERIFICATION_FAILED,
  PERMISSION_NOK_APPID_NOT_PERMITTED,
  PERMISSION_NOK_RPC_NOT_PERMITTED
};
}  // namespace CheckPermissionResult

/**
 * @brief Interface class of policy manager.
 */
class PolicyManager {
  public:
    /**
     * @brief Constructor
     *
     * @param policy_config Policy configuration
     */
    PolicyManager(const PolicyConfiguration& policy_config);

    /**
    * @brief Initialization method
    */
    void Init();

    /**
     * @brief Checking permissions for application whether rpc is allowed.
     *
     * @param app_id  Application identifier
     * @param rpc     Remote procedure call
     *
     * @return result of check permission
     */
    CheckPermissionResult::eType checkPermission(uint32_t app_id,
        const NsSmartDeviceLink::NsSmartObjects::SmartObject& rpc);


  private:
    /**
     * @brief create schema of Policy Table for SDL (non Ford-specific)
     *
     * @return created schema
     **/
    NsSmartDeviceLink::NsSmartObjects::CSmartSchema createSchemaSDL();

    /**
    * @brief Validate poicy table.
    *
    * Validates policy table against smart schema which describes policy table
    *
    * @param policy_table Policy table as smart object
    *
    * @return true if successful; false otherwise
    **/
    bool validatePT(
      const NsSmartDeviceLink::NsSmartObjects::SmartObject& policy_table);

    /**
     * @brief Policy configuration
     */
    const PolicyConfiguration& policy_config_;
};

}  // namespace policies
}  // namespace NsSmartDeviceLink


#endif  // SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_POLICY_MANAGER_H_
