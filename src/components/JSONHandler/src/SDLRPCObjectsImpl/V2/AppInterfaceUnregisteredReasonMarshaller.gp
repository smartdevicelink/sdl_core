%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name AppInterfaceUnregisteredReason_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsSmartDeviceLinkRPCV2::PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
USER_EXIT,0
IGNITION_OFF,1
BLUETOOTH_OFF,2
USB_DISCONNECTED,3
REQUEST_WHILE_IN_NONE_HMI_LEVEL,4
TOO_MANY_REQUESTS,5
DRIVER_DISTRACTION_VIOLATION,6
LANGUAGE_CHANGE,7
MASTER_RESET,8
FACTORY_DEFAULTS,9
APP_UNAUTHORIZED,10
