# New Features

### EXTENDED_POLICY modes:
- The `EXTENDED_POLICY` CMake variable (previously `ENABLE_EXTENDED_POLICY`) now has three possible configurations
  - `HTTP` (previously `ENABLE_EXTENDED_POLICY: OFF`) - #941 
  - `PROPRIETARY` (previously `ENABLE_EXTENDED_POLICY: ON`) - #940
  - `EXTERNAL_PROPRIETARY` (new, fully featured version of `PROPRIETARY` mode) - #942 

### EXTERNAL_PROPRIETARY mode:
#### New policy table update sequence 
A new policy table update flow was created specifically for the `EXTERNAL_PROPRIETARY`  policy mode

- Requirements/Details - #933 
- Diagram - https://user-images.githubusercontent.com/10549248/26896053-9417c604-4b91-11e7-9e47-524c930eb542.png

#### External Policy Manager
As part of the `EXTERNAL_PROPRIETARY` policy mode, the concept of an "external policy manager" is necessary. This policy manager is a separate program which is in charge of encrypting/decrypting policy tables and attaching an HTTP header to the OnSystemRequest payload when performing a Policy Table Update.

As part of this release, a sample application which performs this function was added to this repository for those who wish to implement this new policy mode, and this program can be started along with Core using an included bash script. This sample application does nothing with the policy table snapshot during the encryption and decryption phases, allowing for OEMs to implement their own encryption algorithms in their place.

#### App permissions/User consent
Users can now control what functional groups that they want apps to be able to access, as well as decide whether to enable SDL functionality at all on a device-by-device basis. 

  - Logic was added to allow the user to control what devices are permitted to use SDL functionality - #934 
    - Users are prompted when activating an app on a new device for the first time whether or not to allow the device to use SDL functionality (sequence shown in [this diagram](https://user-images.githubusercontent.com/10549248/26897493-ba378f2c-4b96-11e7-93b0-b24f01c7dc28.png))
  - Logic was added to the Policy Manager to allow the user to control what apps have access to specific functional groups - #939 
    - Users are prompted when activating an app for the first time (or modifying permissions in settings) with information on what access a requested functional group requires. The user responds to determine whether or not to allow this functionality within the new app (sequence shown in [this diagram](https://user-images.githubusercontent.com/10549248/26902322-e6e02bf0-4ba6-11e7-98b3-c285396061a4.png))


#### External user consent
External user consent allows the HMI to define several groups of permissions within the policy table. This allows the user to enable/disable several functional groups at once.

  - The `externalConsentStatus` field is included as part of a GetListOfPermissions response from SDL Core to communicate which groups are activated - #1047 
  - External consent groups can be used to enable sets of functional groups using the `disallowed_by_external_consent_entities_off` field in the Policy Table - #1049 
    - If this external consent group is set to `ON`, all functional groupings with this parameter are allowed by the user - #1048 
    - If this external consent group is set to `OFF`, all functional groupings with this parameter are disallowed by the user - #1050 
  - External consent groups can be used to disable sets of functional groups using the `disallowed_by_external_consent_entities_on` field in the Policy Table
    - If this external consent group is set to `ON`, all functional groupings with this parameter are disallowed by the user
    - If this external consent group is set to `OFF`, all functional groupings with this parameter are allowed by the user

#### Cache Manager Function Implementations
Prior to this release, several functions included in cache_manager.cc were not fully implemented and would not query the local policy table for defined rules and policies. The newly implemented functions for the `EXTERNAL_PROPRIETARY` cache manager are listed below:
 - [CanAppKeepContext()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L310)
 - [CanAppStealFocus()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L363)
 - [GetDefaultHMI()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L374)
 - [ResetUserConsent()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L391)
 - [GetUserPermissionsForDevice()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L406)
 - [GetPreconsentedGroups()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L477)
 - [GetConsentedGroups()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L514)
 - [GetUnconsentedGroups()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L549)
 - [RemoveAppConsentForGroup()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L605)
 - [GetDeviceGroupsFromPolicies()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L799)
 - [SetDeviceData()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L975)
 - [SetUserPermissionsForDevice()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L1008)
 - [ReactOnUserDevConsentForApp()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L1043)
 - [SetUserPermissionsForApp()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L1076)
 - [CountUnconsentedGroups()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L1768)
 - [SetMetaInfo()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L1868) 
 - [IsMetaInfoPresent()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L1884)
 - [SetSystemLanguage()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L1893)
 - [CleanupUnpairedDevices()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L1919)
 - [SetVinValue()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L2175)
 - [ResetPT()](https://github.com/smartdevicelink/sdl_core/blob/release/4.3.0/src/components/policy/policy_external/src/cache_manager.cc#L2309)

#### Security behavior 
Requirements/Details - #937 

### HMI_API additions:

A new RPC was added as part of the implementation of `EXTERNAL_PROPRIETARY` policy mode

  - [DecryptCertificate](https://github.com/smartdevicelink/sdl_core/blob/6283aa26e262d2d16ed1393989d8e0e062dba88d/src/components/interfaces/HMI_API.xml#L2468-L2476) RPC

Several API additions were made as part of the implementation of the external user consent feature

  - [EntityStatus](https://github.com/smartdevicelink/sdl_core/blob/6283aa26e262d2d16ed1393989d8e0e062dba88d/src/components/interfaces/HMI_API.xml#L1263-L1266) enum
  - [ExternalConsentStatus](https://github.com/smartdevicelink/sdl_core/blob/6283aa26e262d2d16ed1393989d8e0e062dba88d/src/components/interfaces/HMI_API.xml#L1268-L1278) struct
  - externalConsentStatus field added to [OnAppPermissionConsent](https://github.com/smartdevicelink/sdl_core/blob/6283aa26e262d2d16ed1393989d8e0e062dba88d/src/components/interfaces/HMI_API.xml#L4170-L4172) and [GetListOfPermissions](https://github.com/smartdevicelink/sdl_core/blob/6283aa26e262d2d16ed1393989d8e0e062dba88d/src/components/interfaces/HMI_API.xml#L4170-L4172)

## Implemented proposals
Two new evolution proposals were implemented in release 4.3.0:

- Add API Patch Version [SDL-0050](https://github.com/smartdevicelink/sdl_evolution/blob/master/proposals/0050-add-api-patch-version.md)
  - A patch version was added to the MOBILE API version, HMI API interface versions, and SyncMsgVersion struct
- External Policy Manager [SDL-0045](https://github.com/smartdevicelink/sdl_evolution/blob/master/proposals/0045-external-policy-manager.md)
  - Details for the implementation of this proposal can be found in the `External Policy Manager` section of these release notes