/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=../src/ALRPCObjectsImpl/SpeechCapabilities_v2Marshaller.inc ../src/ALRPCObjectsImpl/SpeechCapabilities_v2Marshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 5
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 15
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 15
/* maximum key range = 12, duplicates = 0 */

class SpeechCapabilities_v2_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsAppLinkRPC::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline /*ARGSUSED*/
unsigned int
SpeechCapabilities_v2_intHash::hash (register const char *str, register unsigned int len)
{
  return len;
}

const struct NsAppLinkRPC::PerfectHashTable *
SpeechCapabilities_v2_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  4,  0,  0,  7,  0,  0,  0,  0, 12, 13,
       0, 15
    };
  static const struct NsAppLinkRPC::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 17 "../src/ALRPCObjectsImpl/SpeechCapabilities_v2Marshaller.gp"
      {"TEXT",0},
      {""}, {""},
#line 21 "../src/ALRPCObjectsImpl/SpeechCapabilities_v2Marshaller.gp"
      {"SILENCE",4},
      {""}, {""}, {""}, {""},
#line 20 "../src/ALRPCObjectsImpl/SpeechCapabilities_v2Marshaller.gp"
      {"PRE_RECORDED",3},
#line 18 "../src/ALRPCObjectsImpl/SpeechCapabilities_v2Marshaller.gp"
      {"SAPI_PHONEMES",1},
      {""},
#line 19 "../src/ALRPCObjectsImpl/SpeechCapabilities_v2Marshaller.gp"
      {"LHPLUS_PHONEMES",2}
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
