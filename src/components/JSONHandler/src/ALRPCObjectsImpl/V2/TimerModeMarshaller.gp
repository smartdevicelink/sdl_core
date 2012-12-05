%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name TimerMode_intHash
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
UP,0
DOWN,1
NONE,2
