%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name TextFieldName_intHash
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
mainField1,0
mainField2,1
statusBar,2
mediaClock,3
mediaTrack,4
alertText1,5
alertText2,6
