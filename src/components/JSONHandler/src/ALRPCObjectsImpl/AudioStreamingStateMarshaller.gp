%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name AudioStreamingState_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
AUDIBLE,0
NOT_AUDIBLE,1
