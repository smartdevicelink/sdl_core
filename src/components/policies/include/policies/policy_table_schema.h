/**
 * @file policy_table_schema.h
 * @brief Policy table schema header file.
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

#ifndef SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_POLICY_TABLE_SCHEMA_H_
#define SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_POLICY_TABLE_SCHEMA_H_

#include <string>

#include "smart_objects/smart_schema.h"
#include "smart_objects/schema_item.h"
#include "utils/shared_ptr.h"

namespace policies {

class PolicyTableSchema {
  public:
    /**
     * @brief Creates schema for Policy Table
     */
    static NsSmartDeviceLink::NsSmartObjects::CSmartSchema Create(void);


    // TODO(YS): might need to move these constants to the more
    // appropriate place
    /**
     * @brief String constant for "policy_table" section.
     */
    static const char* kStrPolicyTable;

    /**
     * @brief String constant for "module_config" section.
     */
    static const char* kStrModuleConfig;

    /**
     * @brief String constant for "functional_groupings" section.
     */
    static const char* kStrFunctionalGroupings;

    /**
     * @brief String constant for "app_policies" section.
     */
    static const char* kStrAppPolicies;

    /**
     * @brief String constant for "endpoints" section.
     */
    static const char* kStrEndpoints;

    /**
     * @brief String constant for "default" section.
     */
    static const char* kStrDefault;

    /**
     * @brief String constant for "user_consent_prompt" section.
     */
    static const char* kStrUserConsentPrompt;

    /**
     * @brief String constant for "rpcs" section.
     */
    static const char* kStrRpcs;

    /**
     * @brief String constant for "hmi_levels" section.
     */
    static const char* kStrHmiLevels;

    /**
     * @brief String constant for "parameters" section.
     */
    static const char* kStrParameters;

    /**
     * @brief String constant for "groups" section.
     */
    static const char* kStrGroups;

    /**
     * @brief String constant for "nicknames" section.
     */
    static const char* kStrNicknames;

    /**
     * @brief String constant for "priority" section.
     */
    static const char* kStrPriority;

  protected:
    /**
     * @brief Hidden constructor to prevent missuse. Only called from inside.
     */
    PolicyTableSchema(void);

    /**
     * @brief Virtual destructor.
     */
    virtual ~PolicyTableSchema(void) {}

    /**
     * @brief Returns a schema.
     *
     * If schema hasn't been created yet the method creates it and returns
     * a newly created schema.
     *
     * @return If the schema has already been created just returns it,
     *  otherwise creates it and returns it.
     */
    virtual NsSmartDeviceLink::NsSmartObjects::CSmartSchema GetSchema(void);

    /**
     * @brief Creates schema for Policy Table
     *
     * @brief Root schema item of the newly created schema.
     */
    virtual utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem>
      CreateSchema(void);

    /**
     * @brief Creates "module_config" schema item
     */
    virtual utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem>
      CreateModuleConfig(void);

    /**
     * @brief Creates "functional_groupings" schema item
     */
    virtual utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem>
      CreateFunctionalGroupings(void);

    /**
     * @brief Creates "app_policies" schema item
     */
    virtual utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem>
      CreateAppPolicies(void);

    /**
     * @brief Creates "<app_id>" schema item
     */
    virtual utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem>
      CreateAppId(void);

    /**
     * @brief Creates "priority" schema item for <app_id> and "default" sections
     */
    virtual utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem>
      CreatePriority(void);

    /**
     * @brief Creates "default" schema item of "app_policies"
     */
    virtual utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem>
      CreateAppPoliciesDefault(void);

  private:
    /**
     * @brief Contains a schema if it is already been created
     */
    utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> schema_;
};

}  // namespace policies

#endif  // SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_POLICY_TABLE_SCHEMA_H_
