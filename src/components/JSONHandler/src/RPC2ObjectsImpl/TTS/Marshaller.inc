/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=../src/RPC2ObjectsImpl/TTS/Marshaller.inc ../src/RPC2ObjectsImpl/TTS/Marshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 4
#define MIN_WORD_LENGTH 9
#define MAX_WORD_LENGTH 27
#define MIN_HASH_VALUE 9
#define MAX_HASH_VALUE 27
/* maximum key range = 19, duplicates = 0 */

class Marshaller_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct Marshaller::localHash *getPointer (const char *str, unsigned int len);
};

inline /*ARGSUSED*/
unsigned int
Marshaller_intHash::hash (register const char *str, register unsigned int len)
{
  return len;
}

const struct Marshaller::localHash *
Marshaller_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  0,  0,  0,  0,
       0,  0,  0, 17,  0, 19,  0,  0,  0,  0,  0,  0,  0, 27
    };
  static const struct Marshaller::localHash wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 20 "../src/RPC2ObjectsImpl/TTS/Marshaller.gp"
      {"TTS.Speak",10,&Marshaller::mSpeak},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 21 "../src/RPC2ObjectsImpl/TTS/Marshaller.gp"
      {"TTS.SpeakResponse",11,&Marshaller::mSpeakResponse},
      {""},
#line 18 "../src/RPC2ObjectsImpl/TTS/Marshaller.gp"
      {"TTS.GetCapabilities",8,&Marshaller::mGetCapabilities},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 19 "../src/RPC2ObjectsImpl/TTS/Marshaller.gp"
      {"TTS.GetCapabilitiesResponse",9,&Marshaller::mGetCapabilitiesResponse}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        if (len == lengthtable[key])
          {
            register const char *s = wordlist[key].name;

            if (*str == *s && !memcmp (str + 1, s + 1, len - 1))
              return &wordlist[key];
          }
    }
  return 0;
}
