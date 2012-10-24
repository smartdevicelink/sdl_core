%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name AudioType_intHash
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
WAV,0
PCM,1
MP3,2
