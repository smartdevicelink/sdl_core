%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name TBTState_v2_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsAppLinkRPC::PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
ROUTE_UPDATE_REQUEST,0
ROUTE_ACCEPTED,1
ROUTE_REFUSED,2
ROUTE_CANCELLED,3
ETA_REQUEST,4
NEXT_TURN_REQUEST,5
ROUTE_STATUS_REQUEST,6
ROUTE_SUMMARY_REQUEST,7
TRIP_STATUS_REQUEST,8
ROUTE_UPDATE_REQUEST_TIMEOUT,9
