%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name SpeechCapabilities_intHash
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
SAPI_PHONEMES,1
PRE_RECORDED,2
SILENCE,3
