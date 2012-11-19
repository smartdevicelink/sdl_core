%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name DriverDistractionState_intHash
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
DD_ON,0
DD_OFF,1
