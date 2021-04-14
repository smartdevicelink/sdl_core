# Release 4.5.0

## Supported Specifications
- SDL Mobile RPC Spec: [Version 4.5.0](https://github.com/smartdevicelink/rpc_spec/releases/tag/4.5.0)
- SDL Protocol Spec: [Version 5.0.0](https://github.com/smartdevicelink/protocol_spec/releases/tag/5.0.0)

## Implemented Proposals

[Connectivity via iAP-BT and Transport Switch](https://github.com/smartdevicelink/sdl_evolution/blob/master/proposals/0053-Connectivity-via-iAP-BT-and-Transport-Switch.md) - Implementation of a mechanism to change a registered app connected over one transport to another seamlessly.

[Mark public deprecated methods](https://github.com/smartdevicelink/sdl_evolution/blob/master/proposals/0092-Deprecated-interfaces-markup.md) - Implemented a DEPRECATED macro for marking deprecated methods in the project. Using methods marked with this macro will result in a warning being generated.

[Remove QT HMI from SDL Core (Partially Complete)](https://github.com/smartdevicelink/sdl_evolution/blob/master/proposals/0110-remove-qt-hmi-from-sdl-core.md) - The `qt_hmi` component was removed from SDL Core, the QT_HMI_API interface and dbus adapter will be removed in the next major release, due to this aspect of the proposal requiring breaking changes.

[Use Boost Library (Partially Complete)](https://github.com/smartdevicelink/sdl_evolution/blob/master/proposals/0044-use-Boost-library.md) - The `boost` library is now installed as a 3rd party library, this library is currently only used in the refactored message broker component.

## Enhancements
- `DBus` and `libusb` are now dynamically linked, instead of being installed as 3rd-party libraries during the SDL Core build - #2004 

## Bug Fixes
- [Remove OEM Specific references in SDL policy table preload file](https://github.com/smartdevicelink/sdl_core/issues/1252)
- ["resultCode" should be more descriptive than "INVALID_DATA"](https://github.com/smartdevicelink/sdl_core/issues/31)
- [SDL doesn't apply sequence SUSPEND -> OnSDLAwake -> SUSPEND -> IGN_OFF for saving resumption data](https://github.com/smartdevicelink/sdl_core/issues/1395)
- [SDL responds "resultCode: SUCCESS" while dataType:VEHICLEDATA_EXTERNTEMP is VEHICLE_DATA_NOT_AVAILABLE and not in subscribed list store](https://github.com/smartdevicelink/sdl_core/issues/982)
- [Policies SDL should be case-insensetive to "AppID" against listed in policies manager](https://github.com/smartdevicelink/sdl_core/issues/992)
- [App is disconnected due to PROTOCOL_VIOLATION when start audio streaming after rejected 2 times then accepted](https://github.com/smartdevicelink/sdl_core/issues/1004)
- [SDL doesn't set unsuccessful "message" value to "info" param in case HMI responds via single UI.RPC when Interface.IsReady missing](https://github.com/smartdevicelink/sdl_core/issues/997)
- [Navigation SDL does not respond info message in case GENERIC_ERROR watchdog timeout from HMI](https://github.com/smartdevicelink/sdl_core/issues/990)
- [SDL does not respond info message in case GENERIC_ERROR watchdog timeout from HM](https://github.com/smartdevicelink/sdl_core/issues/1012)
- [SDL doesn't send info parameter when result of ResetGlobalProperties is GENERIC_ERROR](https://github.com/smartdevicelink/sdl_core/issues/1016)
- [SDL does not send StopAudioStream() if exit app while Video service and Audio service are starting.](https://github.com/smartdevicelink/sdl_core/issues/1002)
- [APIs AlertManeuver: SDL responds GENERIC_ERROR instead of INVALID_DATA when soft button has Type is Image or Both and Text is whitespace or \t or \n or empty](https://github.com/smartdevicelink/sdl_core/issues/980)
- [IVSU SDL doesn't reject SystemRequest with filenam=IVSU but w/o binary data.](https://github.com/smartdevicelink/sdl_core/issues/976)
- [Memory leaks: SDL does not release memory after sending AddCommand limit exhausted](https://github.com/smartdevicelink/sdl_core/issues/1029)
- [Negative result code send instead of IGNORED for UnsubscribedVehicleData when VehicleInfo IsReady Missing](https://github.com/smartdevicelink/sdl_core/issues/996)
- [API SDL responds "UNSUPPORTED_RESOURCE", success= false in case only have "UNSUPPORTED_RESOURCE" to Navigation.AlertManeuver](https://github.com/smartdevicelink/sdl_core/issues/989)
- [Default app policies are never updated after a PTU](https://github.com/smartdevicelink/sdl_core/issues/1772)
- [Build fails with GCC6+](https://github.com/smartdevicelink/sdl_core/issues/1975)
- [Remote Control test suite fails](https://github.com/smartdevicelink/sdl_core/issues/1993)
- [PoliciesManager allows all requested params in case "parameters" field is empty](https://github.com/smartdevicelink/sdl_core/issues/1873)
- [OnDriverDistraction SDL does not send notification to app right after this app changes level from NONE to any other](https://github.com/smartdevicelink/sdl_core/issues/1881)
- [Protect access to Resumption data during LastState::SaveStateToFileSystem](https://github.com/smartdevicelink/sdl_core/issues/1953)
- [Need to protect cache manager "pt_" from concurrent access](https://github.com/smartdevicelink/sdl_core/issues/1961)
- [Prevent deadlock in EventDispatcherIimpl::raise_event](https://github.com/smartdevicelink/sdl_core/issues/1949)
- [Bluetooth StartService fail after Core restarted](https://github.com/smartdevicelink/sdl_core/issues/1932)
- [Silent error caused by implicit conversion of SmartPointer to integer](https://github.com/smartdevicelink/sdl_core/issues/1834)
- [AOA USB transport buffer size too small](https://github.com/smartdevicelink/sdl_core/issues/1863)
- [Lock screen icon URL should be updated](https://github.com/smartdevicelink/sdl_core/issues/1646)
- [Broken link in README.md for Software Architecture Documentation](https://github.com/smartdevicelink/sdl_core/issues/2091)
- [SDL doesn't send OnPermissionsChange in case of external user consent](https://github.com/smartdevicelink/sdl_core/issues/2072)
- [Build fails when ENABLE_SECURITY=OFF](https://github.com/smartdevicelink/sdl_core/issues/2073)

### Security Related Fixes
- [SDL must start PTU for navi app right after app successfully registration](https://github.com/smartdevicelink/sdl_core/issues/1925)
- [SDL must start PTU for any app except navi right after app successfully request to start first secure service](https://github.com/smartdevicelink/sdl_core/issues/1924)
- [PolicyTableUpdate is failed by any reason and "ForceProtectedService"=ON at .ini file](https://github.com/smartdevicelink/sdl_core/issues/1923)
- [PolicyTableUpdate has NO "certificate" and "ForceProtectedService"=ON at .ini file](https://github.com/smartdevicelink/sdl_core/issues/1922)
- [SDL must respond NACK in case navigation app connected over protocol v2 sends StartService for audio service](https://github.com/smartdevicelink/sdl_core/issues/1912)
- [PolicyTableUpdate has NO "certificate" and "ForceProtectedService"=OFF at .ini file](https://github.com/smartdevicelink/sdl_core/issues/1894)
- [PolicyTableUpdate is failed by any reason and "ForceProtectedService"=OFF at .ini file](https://github.com/smartdevicelink/sdl_core/issues/1891)
- [PolicyTableUpdate is valid and brings "certificate"](https://github.com/smartdevicelink/sdl_core/issues/1888)
