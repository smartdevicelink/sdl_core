/**
 * @file policy_manager.h
 * @brief Policy Manager header file.
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

#include "interfaces/MOBILE_API.h"
#include "policies/policy_configuration.h"
#include "smart_objects/smart_object.h"



namespace NsSmartDeviceLink {
namespace policies {

 /**
 * @brief enumeration of result for asking permissions by application
 **/
namespace PermissionResult {
enum eType {
  /**
   * @brief RPC is allowed.
   */
  PERMISSION_ALLOWED = 0,
  /**
  * @brief Initialization of Policy Manager failed.
  */ 
  PERMISSION_INIT_FAILED,
  /**
   * @brief Verification of Policy Table failed.
   *
   * Policy Table as smart object has failed in verification against schema.
   */
  PERMISSION_PT_VERIFICATION_FAILED,
  /**
   * @brief RPC is allowed by the PT(backend), but disallowed by the user.
   */
  PERMISSION_USER_DISALLOWED,
  /**
   * @brief RPC  is not allowed by the PT(backend).
   */
  PERMISSION_DISALLOWED,
  /**
   * @brief RPC is allowed by PT(backend), but user has not been prompted yet.
   */
  PERMISSION_PENDING_USER_CONSENT
};
}  // namespace PermissionResult

// Namespace is neseccary because it is used for automatic string<->enum
// convertion using schema
namespace Priority {
enum eType {
  /**
   * @brief NONE
   */
  PRIORITY_NONE = 0,
  /**
   * @brief NORMAL
   */
  PRIORITY_NORMAL,
  /**
   * @brief COMMUNICATION
   */
  PRIORITY_COMMUNICATION,
  /**
   * @brief NAVIGATION
   */
  PRIORITY_NAVIGATION,
  /**
   * @brief EMERGENCY
   */
  PRIORITY_EMERGENCY,
  /**
   * @brief Mandatory item. Used for enum<->string convertion
   */
  INVALID_ENUM,
};
}  // namespace Priority


/**
 * @brief Init() result enumeration
 **/
namespace InitResult {
enum eType {
  /**
   * @brief PT file loaded successfully
   **/
  INIT_OK = 0,
  /**
   * @brief Initialization has failed
   *
   * Possible reason: Policy Table file missing, failed verification
   *
   **/
  INIT_FAILED
};
}  // namespace InitResult

/**
 * @brief Enumeration for state wich reflects states of User consent procedure
 *        @TODO (anyone) for future use
 */
enum eType {
  /**
   * @brief User was not asked for consent
   */
  STATE_IDLE = 0,
  /**
   * @brief User was asked just now for consent
   */
  STATE_PENDING_CONSENT,
  /**
   * @brief User answered 'YES' just now
   */
  STATE_CONSENT_YES,
  /**
   * @brief User answered 'NO' just now
   */
  STATE_CONSENT_NO
};

/**
 * @biref Struct contains data of result to return when Policy Manager
 *        is requested for CheckPermission()
 */
struct CheckPermissionResult {
  /**
   * @brief Permission result
  */
  PermissionResult::eType result;
  /**
   * @brief Stored priority for current application
   */
  Priority::eType priority;
};

/**
 * @brief Interface class of policy manager.
 */
class PolicyManager {
  public:
    /**
     * @brief Destructor
     *
     **/
    virtual ~PolicyManager() {}

    /**
    * @brief Initialization method
    *
    * @param config PolicyManager configuration
    */
    virtual InitResult::eType Init(const PolicyConfiguration& config) = 0;

    /**
     * @brief Checking permissions for application whether rpc is allowed.
     *
     * @param app_id  Application identifier
     * @param rpc     Remote procedure call
     * @param hmi_status  HMI level status
     *
     * @return result of check permission
     */
    virtual CheckPermissionResult CheckPermission(uint32_t app_id,
        const NsSmartDeviceLink::NsSmartObjects::SmartObject& rpc,
        const mobile_apis::HMILevel::eType hmi_status) = 0;
};

}  // namespace policies
}  // namespace NsSmartDeviceLink


#endif  // SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_POLICY_MANAGER_H_
