%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name HmiZoneCapabilities_intHash
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
FRONT,0
BACK,1
