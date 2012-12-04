%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name TextAlignment_intHash
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
LEFT_ALIGNED,0
RIGHT_ALIGNED,1
CENTERED,2
