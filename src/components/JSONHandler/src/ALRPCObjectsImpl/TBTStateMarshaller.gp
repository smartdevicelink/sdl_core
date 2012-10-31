%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name TBTState_intHash
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
ROUTE_UPDATE_REQUEST,0
ROUTE_ACCEPTED,1
ROUTE_REFUSED,2
ROUTE_CANCELLED,3
