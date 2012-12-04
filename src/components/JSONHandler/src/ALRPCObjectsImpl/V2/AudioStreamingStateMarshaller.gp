%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name AudioStreamingState_intHash
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
AUDIBLE,0
ATTENUATED,1
NOT_AUDIBLE,2
