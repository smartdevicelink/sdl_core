%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name MediaClockFormat_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsAppLinkRPCV1::PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
CLOCK1,0
CLOCK2,1
CLOCKTEXT1,2
CLOCKTEXT2,3
CLOCKTEXT3,4
