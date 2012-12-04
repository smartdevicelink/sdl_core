%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name HMILevel_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsAppLinkRPCV1::PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
FULL,0
LIMITED,1
BACKGROUND,2
NONE,3
