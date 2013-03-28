%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name SystemAction_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsSmartDeviceLinkRPCV2::PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
DEFAULT_ACTION,0
STEAL_FOCUS,1
KEEP_CONTEXT,2
