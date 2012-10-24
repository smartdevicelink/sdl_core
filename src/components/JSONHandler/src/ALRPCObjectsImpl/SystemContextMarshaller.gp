%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name SystemContext_intHash
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
MAIN,0
VRSESSION,1
MENU,2
HMI_OBSCURED,3
ALERT,4
