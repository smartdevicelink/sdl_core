%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name PermissionStatus_intHash
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
ALLOWED,0
DISALLOWED,1
USER_DISALLOWED,2
USER_CONSENT_PENDING,3
