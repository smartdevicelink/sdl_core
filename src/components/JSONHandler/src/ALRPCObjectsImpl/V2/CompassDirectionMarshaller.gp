%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name CompassDirection_intHash
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
NORTH,0
NORTHWEST,1
WEST,2
SOUTHWEST,3
SOUTH,4
SOUTHEAST,5
EAST,6
NORTHEAST,7
