%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name VehicleDataResultCode_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsAppLinkRPC::PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
SUCCESS,0
DISALLOWED,1
USER_DISALLOWED,2
INVALID_ID,3
VEHICLE_DATA_NOT_AVAILABLE,4
DATA_ALREADY_SUBSCRIBED,5
DATA_NOT_SUBSCRIBED,6
IGNORED,7
