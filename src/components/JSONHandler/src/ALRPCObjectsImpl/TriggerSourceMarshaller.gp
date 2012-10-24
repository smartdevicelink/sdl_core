%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name TriggerSource_intHash
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
MENU,0
VR,1
