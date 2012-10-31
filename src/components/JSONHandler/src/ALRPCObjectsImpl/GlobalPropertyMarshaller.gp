%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name GlobalProperty_intHash
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
HELPPROMPT,0
TIMEOUTPROMPT,1
