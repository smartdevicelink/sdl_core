%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name SingleTirePressureStatus_intHash
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
UNKNOWN,0
NORMAL,1
LOW,2
FAULT,3
NOT_SUPPORTED,4
