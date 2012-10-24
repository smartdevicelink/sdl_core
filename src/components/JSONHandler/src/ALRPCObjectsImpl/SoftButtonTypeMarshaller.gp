%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name SoftButtonType_intHash
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
TEXT,0
IMAGE,1
BOTH,2
