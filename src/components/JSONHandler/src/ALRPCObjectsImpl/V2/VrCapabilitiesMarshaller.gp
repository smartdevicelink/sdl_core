%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name VrCapabilities_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsAppLinkRPCV2::PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
TEXT,0
