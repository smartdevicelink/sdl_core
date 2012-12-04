%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name DriverDistractionState_intHash
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
DD_ON,0
DD_OFF,1
