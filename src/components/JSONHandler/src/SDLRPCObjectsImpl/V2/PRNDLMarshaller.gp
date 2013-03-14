%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name PRNDL_intHash
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
PARK,0
REVERSE,1
NEUTRAL,2
LOWGEAR,3
FORWARD_DRIVE_2,4
FORWARD_DRIVE_3,5
FORWARD_DRIVE_4,6
FORWARD_DRIVE_5,7
FORWARD_DRIVE_6,8
SECOND_GEAR_HOLD,9
THIRD_GEAR_HOLD,10
FOURTH_GEAR_HOLD,11
FIFTH_GEAR_HOLD,12
SIXTH_GEAR_HOLD,13
