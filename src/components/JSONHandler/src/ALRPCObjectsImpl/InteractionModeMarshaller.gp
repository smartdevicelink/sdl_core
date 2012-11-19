%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name InteractionMode_intHash
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
MANUAL_ONLY,0
VR_ONLY,1
BOTH,2
