%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name AppType_intHash
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
DEFAULT,0
COMMUNICATION,1
MEDIA,2
MESSAGING,3
NAVIGATION,4
INFORMATION,5
SOCIAL,6
BACKGROUND_PROCESS,7
TESTING,8
SYSTEM,9
