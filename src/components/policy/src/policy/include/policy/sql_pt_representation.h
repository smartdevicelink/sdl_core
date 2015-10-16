/*
 Copyright (c) 2013, Ford Motor Company
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

#ifndef SRC_COMPONENTS_POLICY_INCLUDE_POLICY_SQL_PT_REPRESENTATION_H_
#define SRC_COMPONENTS_POLICY_INCLUDE_POLICY_SQL_PT_REPRESENTATION_H_

#include <string>
#include <vector>
#include "policy/pt_representation.h"
#include "rpc_base/rpc_base.h"
#include "types.h"

namespace policy_table = rpc::policy_table_interface_base;

namespace utils {
namespace dbms {
class SQLDatabase;
} // namespace dbms
} // namespace utils


namespace policy {

class SQLPTRepresentation : public virtual PTRepresentation {
  public:
    SQLPTRepresentation();
    ~SQLPTRepresentation();
    virtual void CheckPermissions(const PTString& app_id,
        const PTString& hmi_level,
        const PTString& rpc,
        CheckPermissionResult& result);

    virtual bool IsPTPreloaded();
    virtual int IgnitionCyclesBeforeExchange();
    virtual int KilometersBeforeExchange(int current);
    virtual bool SetCountersPassedForSuccessfulUpdate(int kilometers,
        int days_after_epoch);
    virtual int DaysBeforeExchange(int current);
    virtual void IncrementIgnitionCycles();
    virtual void ResetIgnitionCycles();
    virtual int TimeoutResponse();
    virtual bool SecondsBetweenRetries(std::vector<int>* seconds);
    virtual bool RefreshDB();
    virtual const VehicleInfo GetVehicleInfo() const;

    virtual std::vector<UserFriendlyMessage> GetUserFriendlyMsg(
      const std::vector<std::string>& msg_codes, const std::string& language);

    virtual EndpointUrls GetUpdateUrls(int service_type);
    virtual std::string GetLockScreenIconUrl() const;
    virtual int GetNotificationsNumber(const std::string& priority);
    virtual bool GetPriority(const std::string& policy_app_id,
                             std::string* priority);
    InitResult Init();
    bool Close();
    bool Clear();
    bool Drop();
    virtual void WriteDb();
    virtual utils::SharedPtr<policy_table::Table> GenerateSnapshot() const;
    virtual bool Save(const policy_table::Table& table);
    bool GetInitialAppData(const std::string& app_id, StringArray* nicknames =
                             NULL,
                           StringArray* app_hmi_types = NULL);
    bool GetFunctionalGroupings(policy_table::FunctionalGroupings& groups);

#ifdef BUILD_TESTS
    uint32_t open_counter() {
    return open_counter_;
    }
#endif // BUILD_TESTS
  protected:
    virtual void GatherModuleMeta(policy_table::ModuleMeta* meta) const;
    virtual void GatherModuleConfig(policy_table::ModuleConfig* config) const;
    virtual bool GatherUsageAndErrorCounts(
      policy_table::UsageAndErrorCounts* counts) const;
    virtual void GatherDeviceData(policy_table::DeviceData* data) const;
    virtual bool GatherFunctionalGroupings(
      policy_table::FunctionalGroupings* groups) const;
    virtual bool GatherConsumerFriendlyMessages(
      policy_table::ConsumerFriendlyMessages* messages) const;
    virtual bool GatherApplicationPoliciesSection(
      policy_table::ApplicationPoliciesSection* policies) const;

    bool GatherAppGroup(const std::string& app_id,
                        policy_table::Strings* app_groups) const;
    bool GatherAppType(const std::string& app_id,
                       policy_table::AppHMITypes* app_types) const;
    bool GatherRequestType(const std::string& app_id,
                           policy_table::RequestTypes* request_types) const;
    bool GatherNickName(const std::string& app_id,
                        policy_table::Strings* nicknames) const;

    virtual bool SaveApplicationCustomData(const std::string& app_id,
                                   bool is_revoked,
                                   bool is_default,
                                   bool is_predata);

    virtual bool SaveModuleMeta(const policy_table::ModuleMeta& meta);
    virtual bool SaveModuleConfig(const policy_table::ModuleConfig& config);
    virtual bool SaveUsageAndErrorCounts(
      const policy_table::UsageAndErrorCounts& counts);
    virtual bool SaveDeviceData(const policy_table::DeviceData& devices);
    virtual bool SaveFunctionalGroupings(
      const policy_table::FunctionalGroupings& groups);
    virtual bool SaveConsumerFriendlyMessages(
      const policy_table::ConsumerFriendlyMessages& messages);
    virtual bool SaveApplicationPoliciesSection(
      const policy_table::ApplicationPoliciesSection& policies);
    virtual bool SaveSpecificAppPolicy(
      const policy_table::ApplicationPolicies::value_type& app);
    virtual bool SaveDevicePolicy(const policy_table::DevicePolicy& device);

    virtual bool SaveMessageString(const std::string& type,
                                   const std::string& lang,
                                   const policy_table::MessageString& strings);

    bool SaveAppGroup(const std::string& app_id,
                      const policy_table::Strings& app_groups);
    bool SaveNickname(const std::string& app_id,
                      const policy_table::Strings& nicknames);
    bool SaveAppType(const std::string& app_id,
                     const policy_table::AppHMITypes& types);
    bool SaveRequestType(const std::string& app_id,
                         const policy_table::RequestTypes& types);

  public:
    bool UpdateRequired() const;
    void SaveUpdateRequired(bool value);

    bool IsApplicationRepresented(const std::string& app_id) const;
    bool CopyApplication(const std::string& source,
                         const std::string& destination);

    bool IsApplicationRevoked(const std::string& app_id) const;
    virtual bool IsDefaultPolicy(const std::string& app_id) const;
    virtual bool IsPredataPolicy(const std::string& app_id) const;
    virtual bool SetDefaultPolicy(const std::string& app_id);
    virtual void SetPreloaded(bool value);

    virtual bool SetVINValue(const std::string& value);

    virtual utils::dbms::SQLDatabase* db() const;
    virtual bool SetIsDefault(const std::string& app_id, bool is_default) const;


    void RemoveDB() const OVERRIDE;
    virtual bool IsDBVersionActual() const OVERRIDE;
    virtual bool UpdateDBVersion() const OVERRIDE;

  private:
    static const std::string kDatabaseName;
    utils::dbms::SQLDatabase* db_;

#ifdef BUILD_TESTS
    uint32_t open_counter_;
#endif // BUILD_TESTS

    /**
     * @brief Calculates DB version from current schema
     * @return version
     */
    const int32_t GetDBVersion() const;
    bool SaveRpcs(int64_t group_id, const policy_table::Rpc& rpcs);
    bool SaveServiceEndpoints(const policy_table::ServiceEndpoints& endpoints);
    bool SaveSecondsBetweenRetries(
      const policy_table::SecondsBetweenRetries& seconds);
    bool SaveNumberOfNotificationsPerMinute(
      const policy_table::NumberOfNotificationsPerMinute& notifications);
    bool SaveMessageType(const std::string& type);
    bool SaveLanguage(const std::string& code);
};
}  //  namespace policy

#endif  // SRC_COMPONENTS_POLICY_INCLUDE_POLICY_SQL_PT_REPRESENTATION_H_
