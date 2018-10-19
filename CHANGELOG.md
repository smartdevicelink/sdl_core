# Release 5.0..0

## Supported Specifications
- [SDL Mobile RPC Spec: Version 5.0.0](https://github.com/smartdevicelink/rpc_spec/releases/tag/5.0.0)
- [SDL Protocol Spec: Version 5.1.0](https://github.com/smartdevicelink/protocol_spec/releases/tag/5.1.0)

## Implemented Proposals / New Features

- [[SDL 0043] Move to the new C++11 standard](https://github.com/smartdevicelink/sdl_core/issues/1493) - C++11 unique_ptr, shared_ptr, atomic utils implemented

- [[SDL 0044] Use Boost library in SDL](https://github.com/smartdevicelink/sdl_core/issues/1523) - Boost Locks, Date/Time, and Filesystem utils implemented

- [[SDL 0122] Handling VR help requests when application does not send VR help prompt](https://github.com/smartdevicelink/sdl_core/issues/2154)

- [[SDL 0160] Remote Control Radio Parameter Update](https://github.com/smartdevicelink/sdl_core/issues/2162)

- [[SDL 0181] SDL shall not put RC applications to HMI level NONE when user disables RC in HMI](https://github.com/smartdevicelink/sdl_core/issues/2377)

- [[SDL 0182] Audio Source AM/FM/XM/DAB](https://github.com/smartdevicelink/sdl_core/issues/2338)

- [[SDL 0165] Remote Control  Lights  More Names and Status Values](https://github.com/smartdevicelink/sdl_core/issues/2172)

- [[SDL 0099] New remote control modules (LIGHT, AUDIO, HMI_SETTINGS) and parameters (SIS Data)](https://github.com/smartdevicelink/sdl_core/issues/1798)

- [[SDL 0178] Interior Vehicle Data Subscription Management and Data Caching in SDL](https://github.com/smartdevicelink/sdl_core/issues/2318)

- [[SDL 0153] Support for Short and Full UUID App ID](https://github.com/smartdevicelink/sdl_core/issues/2159)

- [[SDL 0170] SDL behavior in case of LOW_VOLTAGE event](https://github.com/smartdevicelink/sdl_core/issues/2233)

- [[SDL 0064] Choice-VR optional](https://github.com/smartdevicelink/sdl_core/issues/2160)

- [[SDL 0175] Updating DOP value range for GPS notification](https://github.com/smartdevicelink/sdl_core/issues/2317)

- [[SDL 0089] Mobile API versioning](https://github.com/smartdevicelink/sdl_core/issues/2248)

- [[SDL 0150] Enhancing onHMIStatus with a New Parameter for Video Streaming State](https://github.com/smartdevicelink/sdl_core/issues/2129)

- [[SDL 0109] SetAudioStreamingIndicator RPC](https://github.com/smartdevicelink/sdl_core/issues/2059)

- [[SDL 0041] Provide AppIcon resumption across app registration requests](https://github.com/smartdevicelink/sdl_core/issues/1456)

- [[SDL 0139] Clarification of audio format details of AudioPassThru](https://github.com/smartdevicelink/sdl_core/issues/2014)

- [[SDL 0105] Remote Control - Seat](https://github.com/smartdevicelink/sdl_core/issues/1860)

- [[SDL 0172] Remote Control - Update OnRCStatus with a new allowed parameter](https://github.com/smartdevicelink/sdl_core/issues/2244)

- [[SDL 0106] Remote Control - OnRCStatus notification](https://github.com/smartdevicelink/sdl_core/issues/1930)

- [[SDL 0145] Driver Distraction Notification Upon Registration](https://github.com/smartdevicelink/sdl_core/issues/2054)

- [[SDL 0102] New vehicle data - ElectronicParkBrakeStatus ](https://github.com/smartdevicelink/sdl_core/issues/1815)

- [[SDL 0110] Remove QT HMI from SDL Core](https://github.com/smartdevicelink/sdl_core/issues/1857) - Fully removed from project

- [[SDL 0141] Supporting simultaneous multiple transports](https://github.com/smartdevicelink/sdl_core/issues/2065)

- [[SDL 0149] Add capability to disable resumption based on app type and transport type](https://github.com/smartdevicelink/sdl_core/issues/2130)

- [[SDL 0107] New vehicle data - TurnSignal](https://github.com/smartdevicelink/sdl_core/issues/1859)

- [[SDL 0085] SubMenu Icon](https://github.com/smartdevicelink/sdl_core/issues/1747)

- [[SDL 0168] RPC design refactoring](https://github.com/smartdevicelink/sdl_core/issues/2189)

- [[SDL 0120] GetSystemTime RPC](https://github.com/smartdevicelink/sdl_core/issues/1963)

- [[SDL 0040] DTLS encryption](https://github.com/smartdevicelink/sdl_core/issues/1479)

- [[SDL 0014] Adding Audio File Playback to TTSChunk](https://github.com/smartdevicelink/sdl_core/issues/1277)

- [[SDL 0063] Display name parameter](https://github.com/smartdevicelink/sdl_core/issues/1589)

- [[SDL 0083] Expandable Design for Proprietary Data Exchange](https://github.com/smartdevicelink/sdl_core/issues/1734)

- [[SDL 0042] SDL must transfer RPCs with invalid image reference parameters to the HMI](https://github.com/smartdevicelink/sdl_core/issues/1428)

- [[SDL 0097] Tire pressure additions](https://github.com/smartdevicelink/sdl_core/issues/1766)

- [[SDL 0062] Template images](https://github.com/smartdevicelink/sdl_core/issues/1588)

- [[SDL 0037] Expand Mobile putfile RPC](https://github.com/smartdevicelink/sdl_core/issues/1455)

- [[SDL 0164] Modernize Ubuntu Support](https://github.com/smartdevicelink/sdl_core/issues/2170)

- [[SDL 0162] Define Handling of Duplicate Correlation IDs](https://github.com/smartdevicelink/sdl_core/issues/2169)

- [[SDL 0151] ImageFieldName for SecondaryImage](https://github.com/smartdevicelink/sdl_core/issues/2099)

- [[SDL 0082] New vehicle data - EngineOilLife](https://github.com/smartdevicelink/sdl_core/issues/1733)

- [[SDL 0147] Template Improvements: Color Scheme](https://github.com/smartdevicelink/sdl_core/issues/2082)

- [[SDL 0096] Deliver build configuration](https://github.com/smartdevicelink/sdl_core/issues/1765)

- [[SDL 0072] New vehicle data - FuelRange](https://github.com/smartdevicelink/sdl_core/issues/1656)

- [Inclusion of PLAY_PAUSE as a subscribable button](https://github.com/smartdevicelink/sdl_core/pull/930)

## Bug Fixes

- [SDL process RC requests with resultCode DISALLOWED after IGN_OFF and IGN_ON](https://github.com/smartdevicelink/sdl_core/issues/2670)

- [SDL stop sending OnInteriorVehicleData notifications after the app sent a GetInteriorVehicleData without optional parameter subscribe](https://github.com/smartdevicelink/sdl_core/issues/2657)

- [Make Size() function thread safe to avoid core dump](https://github.com/smartdevicelink/sdl_core/issues/2603)

- [SDL does not cut parameters not related to defined module type from RC.SetInteriorVD](https://github.com/smartdevicelink/sdl_core/issues/2664)

- [Add missing PRNDL enum value to the HMI API ](https://github.com/smartdevicelink/sdl_core/issues/2431)

- [EXTERNAL_PROPRIETARY user consent is broken when permissions are in default object](https://github.com/smartdevicelink/sdl_core/issues/2146)

- [Processing invalid PT after cutting off unknown_parameter or unknown_RPC](https://github.com/smartdevicelink/sdl_core/issues/1921)

- [SDL does not check for non-mandatory parameters](https://github.com/smartdevicelink/sdl_core/issues/2443)

- [SDL must close only session in case mobile app does not answer on Heartbeat_request](https://github.com/smartdevicelink/sdl_core/issues/1893)

- [App does not activate when policies are disabled](https://github.com/smartdevicelink/sdl_core/issues/842)

- [Pure virtual method called during SDL shutdown](https://github.com/smartdevicelink/sdl_core/issues/2433)

- [SDL build is failing when ENABLE_LOG=OFF](https://github.com/smartdevicelink/sdl_core/issues/2341)

- [Wrong policy permissions calculation in EXTERNAL_PROPRIETARY flow](https://github.com/smartdevicelink/sdl_core/issues/2405)

- [Functional groupings parameters has max size of 24](https://github.com/smartdevicelink/sdl_core/issues/2470)

- [AudioPassThru does not send audio data in raw PCM data](https://github.com/smartdevicelink/sdl_core/issues/1928)

- [Deprecated GetDataOnSessionKey() cannot be overloaded in cross-compiling](https://github.com/smartdevicelink/sdl_core/issues/2137)

- [V5 Protocol Messages do not encrypt payload of Control Frame Message Types](https://github.com/smartdevicelink/sdl_core/issues/2142)

- [Remove TLS Handshake App Name String Compare](https://github.com/smartdevicelink/sdl_core/issues/1617)

- [The reference of wild pointer cause to memory leak](https://github.com/smartdevicelink/sdl_core/issues/860)

- [Build failing on Fedora 25](https://github.com/smartdevicelink/sdl_core/issues/1495)

- [Add missing RPC changes: SpaceAvailable & CurrentTemperatureAvailable](https://github.com/smartdevicelink/sdl_core/pull/2588)

- [Invalid memory accesses detected by valgrind](https://github.com/smartdevicelink/sdl_core/issues/2177)

- [Memory leaks in media_manager_test unit tests](https://github.com/smartdevicelink/sdl_core/issues/2274)

- [Empty moduleType disallows all RPC requests after ignition cycle](https://github.com/smartdevicelink/sdl_core/issues/2547)

- [Feature/boost datetime implementation](https://github.com/smartdevicelink/sdl_core/pull/2278)

- [Flush logger before deleting message loop thread](https://github.com/smartdevicelink/sdl_core/pull/2532)

- [SDL Proxy crash happens when receiving SDLOnAppInterfaceUnregistered notification with reason -1 (INVALID_ENUM)](https://github.com/smartdevicelink/sdl_core/issues/2300)

- [OnHMIStatus returns null hmiLevel if RPC has invalid/incomplete parameters](https://github.com/smartdevicelink/sdl_core/issues/2285)

- [Invalid memory access in CacheManager::CheckPermissions()](https://github.com/smartdevicelink/sdl_core/issues/2298)

- [Cert processing invalid, module certificate should be updated when a policy table update occurs](https://github.com/smartdevicelink/sdl_core/issues/2191)

- [Cert processing invalid, CertificatePath and CACertificatePath keywords are nonfunctional](https://github.com/smartdevicelink/sdl_core/issues/2190)

- [RegisterAppInterface Display Capabilities Segfault](https://github.com/smartdevicelink/sdl_core/issues/2220)

- [Duplicate "PendingRequestsAmount" entries in smartDeviceLink.ini](https://github.com/smartdevicelink/sdl_core/issues/1790)

## Enhancements

- [PrintSmartObject function can not print to log file](https://github.com/smartdevicelink/sdl_core/issues/2574)

## Best Practice

- [AllowDeviceToConnect is never used](https://github.com/smartdevicelink/sdl_core/issues/494)

- [Fix UnsubscribeWayPoints typo](https://github.com/smartdevicelink/sdl_core/pull/2504)

- [Namespace capitalization](https://github.com/smartdevicelink/sdl_core/issues/33)

- [Remove "SetIcon" Files](https://github.com/smartdevicelink/sdl_core/issues/1594)

- [Initialization (et al) method return values](https://github.com/smartdevicelink/sdl_core/issues/34)

- [Fix misspellings of "subscription"](https://github.com/smartdevicelink/sdl_core/issues/736)


## Documentation

- [Add FAQ link to README.md](https://github.com/smartdevicelink/sdl_core/pull/1366)

- [Broken Links in SDD Documentation](https://github.com/smartdevicelink/sdl_core/issues/2107)

- [Alter build instructions for slight clarity improvements](https://github.com/smartdevicelink/sdl_core/pull/218)

- [Add third_party file](https://github.com/smartdevicelink/sdl_core/issues/2610)