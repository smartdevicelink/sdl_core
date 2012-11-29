%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name VrCapabilities_v2_intHash
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
TEXT,0
