%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name ButtonName_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
OK,0
SEEKLEFT,1
SEEKRIGHT,2
TUNEUP,3
TUNEDOWN,4
PRESET_0,5
PRESET_1,6
PRESET_2,7
PRESET_3,8
PRESET_4,9
PRESET_5,10
PRESET_6,11
PRESET_7,12
PRESET_8,13
PRESET_9,14
CUSTOM_BUTTON,15
