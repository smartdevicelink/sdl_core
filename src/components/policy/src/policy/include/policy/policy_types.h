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

#ifndef SRC_COMPONENTS_POLICY_INCLUDE_POLICY_POLICY_TYPES_H_
#define SRC_COMPONENTS_POLICY_INCLUDE_POLICY_POLICY_TYPES_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include "utils/shared_ptr.h"
#include "utils/helpers.h"

namespace policy {

// TODO(PV): specify errors
enum PolicyErrorEnum {
};

const std::string kDefaultDeviceMacAddress = "00:00:00:00:00:00";
const std::string kDefaultDeviceName = "MyDevice";
const std::string kDefaultDeviceConnectionType = "UNKNOWN";

/**
 * @brief Constants for special ids in application policies section of
 * policy table
 */
const std::string kPreDataConsentId = "pre_DataConsent";
const std::string kDefaultId = "default";
const std::string kDeviceId = "device";

/*
 * @brief Status of policy table update
 */
enum PolicyTableStatus {
    StatusUpToDate = 0,
    StatusUpdatePending,
    StatusUpdateRequired,
    StatusUnknown
};

// Code generator uses String class name, so this typedef was renamed to PTSring
typedef std::string PTString;
typedef std::vector<uint8_t> BinaryMessage;
typedef utils::SharedPtr<BinaryMessage> BinaryMessageSptr;

typedef std::string HMILevel;
typedef std::string Parameter;
typedef std::string RpcName;
typedef std::vector<std::string> RPCParams;

typedef std::map<std::string, std::set<policy::HMILevel> > HMIPermissions;
typedef std::map<std::string, std::set<policy::Parameter> > ParameterPermissions;

struct RpcPermissions {
    HMIPermissions hmi_permissions;
    ParameterPermissions parameter_permissions;
};

typedef std::map<RpcName, RpcPermissions> Permissions;

/**
 * @brief Typedef for use with AllowApp request/notification
 */
typedef std::vector<std::string> PermissionsList;

/**
 * @brief Typedef for getting initial application data, e.g. nickname list
 */
typedef std::vector<std::string> StringArray;

enum PermitResult {
    kRpcAllowed = 0,
    kRpcDisallowed,
    kRpcUserDisallowed
};

/**
  * @struct Stores result of check:
  * if HMI Level was allowed for RPC to work in
  * and list of parameters allowed for RPC if specified in PT.
  */
struct CheckPermissionResult {
    CheckPermissionResult()
        : hmi_level_permitted(kRpcDisallowed) {
    }

    PermitResult hmi_level_permitted;
    std::vector<PTString> list_of_allowed_params;
    std::vector<PTString> list_of_disallowed_params;
    std::vector<PTString> list_of_undefined_params;
};

/**
  @struct Holds Url string and optional policy app id.
  */
struct EndpointData {
    explicit EndpointData(const std::string& url_string = "")
        : app_id("default") {
      if (false == url_string.empty()) {
        url.push_back(url_string);
      }
    }
    std::vector<std::string> url;
    std::string app_id;
};

typedef std::vector<EndpointData> EndpointUrls;

/**
 * @brief Struct contains device data to be used for dialogs, generation of IDs
 */
struct DeviceParams {
    DeviceParams()
        : device_name(kDefaultDeviceName),
          device_mac_address(kDefaultDeviceMacAddress),
          device_connection_type(kDefaultDeviceConnectionType),
          device_handle(0) {
    }

    std::string device_name;
    std::string device_mac_address;
    std::string device_connection_type;
    uint32_t device_handle;
};

/**
 * @brief User consent for device data usage
 */
enum DeviceConsent {
    kDeviceAllowed = 0,
    kDeviceDisallowed,
    kDeviceHasNoConsent
};

/**
 * @brief Struct contains parameters, which can be received during application
 * registration and should be stored in policy table
 */
struct DeviceInfo {
    DeviceInfo()
        : max_number_rfcom_ports(0) {
    }

    std::string hardware;
    std::string firmware_rev;
    std::string os;
    std::string os_ver;
    std::string carrier;
    uint32_t max_number_rfcom_ports;
    std::string connection_type;

    void AdoptDeviceType(const std::string& deviceType) {
      connection_type = "USB_serial_number";
      using namespace helpers;
      static const std::string bluetooth("BLUETOOTH");
      static const std::string wifi("WIFI");
      if (Compare<std::string, EQ, ONE>(deviceType, bluetooth, wifi)) {
        connection_type.assign("BTMAC");
      }
    }
};

/**
 * @brief User consent for functional group
 */
enum GroupConsent {
    kGroupAllowed = 0,
    kGroupDisallowed,
    kGroupUndefined
};

/**
 * @brief Contains user permission for RPC functional group with specific name
 * and id from DB
 */
struct FunctionalGroupPermission {
    FunctionalGroupPermission()
        : group_id(0),
          state(kGroupUndefined) {
    }

    bool operator ==(const FunctionalGroupPermission& rhs) {
      if (this->group_id == rhs.group_id &&
          this->group_alias == rhs.group_alias &&
          this->group_name == rhs.group_name) {
        return true;
      }
      return false;
    }

    std::string group_alias;
    std::string group_name;
    int32_t group_id;
    GroupConsent state;
};

/**
 * @brief Stores data to be sent to HMI on application permissions change
 */
struct AppPermissions {

    AppPermissions(const std::string& app_id)
        : application_id(app_id),
          isAppPermissionsRevoked(false),
          appRevoked(false),
          appPermissionsConsentNeeded(false),
          appUnauthorized(false),
          requestTypeChanged(false) {
    }

    std::string application_id;
    bool isAppPermissionsRevoked;
    std::vector<policy::FunctionalGroupPermission> appRevokedPermissions;
    bool appRevoked;
    bool appPermissionsConsentNeeded;
    bool appUnauthorized;
    bool isSDLAllowed;
    std::string priority;
    DeviceParams deviceInfo;
    bool requestTypeChanged;
    std::vector<std::string> requestType;
};

/**
 * @brief Contains parameters for user-defined consent for appication
 * functional groups on given device
 */
struct PermissionConsent {
    std::string device_id;
    std::string policy_app_id;
    std::vector<FunctionalGroupPermission> group_permissions;
    std::string consent_source;
};

/**
 * @brief Contain data for GetUserFriendyMessage response
 */
struct UserFriendlyMessage {
    std::string message_code;
};

/**
 * @brief Types of functional groups in policy table
 */
enum GroupType {
    kTypeDefault = 0,      // groups assigned to 'default' permissions section
    kTypeAllowed,          // groups allowed by user for specific application
    kTypeDisallowed,       // groups disallowed by user for specific application
    kTypeUnconsented,      // groups disallowed by default but consent may be changed by user
    kTypePreconsented,     // groups allowed for specific application without
                           // user consent by default (could be changed by user)
    kTypeGeneral,          // groups assigned to specific application
    kTypePreDataConsented, // groups assigned to 'pre_DataConsent' permissions
                           // section
    kTypeDevice            // groups assigned to 'device' permissions section
};

/**
 * @brief Array of functional group id from DB
 */
typedef std::vector<int32_t> FunctionalGroupIDs;

/**
 * @brief Array of functional group ids sorted by types
 */
typedef std::map<GroupType, FunctionalGroupIDs> FunctionalIdType;

/**
 * @brief Array of functional group ids binded to user_consent_prompt (e.g.
 * VehicleData) and group name (e.g. VehicleData-4)
 */
typedef std::map<uint32_t, std::pair<std::string, std::string> > FunctionalGroupNames;

/**
 * @brief Array of device ids, which are an identifiers in policy table
 */
typedef std::vector<std::string> DeviceIds;

/**
 * @brief Counters that calculated on receiving of succesful update
 */
enum Counters {
  KILOMETERS,
  DAYS_AFTER_EPOCH
};

/**
 * @struct Vehicle information
 */
struct VehicleInfo {
  std::string vehicle_make;
  std::string vehicle_model;
  std::string vehicle_year;
};

}  //  namespace policy

#endif  //  SRC_COMPONENTS_POLICY_INCLUDE_POLICY_POLICY_TYPES_H_
