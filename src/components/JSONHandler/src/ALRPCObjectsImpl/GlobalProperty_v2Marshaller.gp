%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name GlobalProperty_v2_intHash
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
HELPPROMPT,0
TIMEOUTPROMPT,1
VRHELPTITLE,2
VRHELPITEMS,3
