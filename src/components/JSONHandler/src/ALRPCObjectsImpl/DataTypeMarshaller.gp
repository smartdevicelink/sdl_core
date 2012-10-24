%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name DataType_intHash
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
HEX,0
BINARY,1
