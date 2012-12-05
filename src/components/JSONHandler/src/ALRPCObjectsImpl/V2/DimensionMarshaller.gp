%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name Dimension_intHash
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
NO_FIX,0
2D,1
3D,2
