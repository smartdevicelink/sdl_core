/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=../src/ALRPCObjectsImpl/V2/AudioStreamingStateMarshaller.inc ../src/ALRPCObjectsImpl/V2/AudioStreamingStateMarshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 3
#define MIN_WORD_LENGTH 7
#define MAX_WORD_LENGTH 11
#define MIN_HASH_VALUE 7
#define MAX_HASH_VALUE 11
/* maximum key range = 5, duplicates = 0 */
namespace NsAppLinkRPCV2
{
class AudioStreamingState_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsAppLinkRPCV2::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline /*ARGSUSED*/
unsigned int
AudioStreamingState_intHash::hash (register const char *str, register unsigned int len)
{
  return len;
}

const struct NsAppLinkRPCV2::PerfectHashTable *
AudioStreamingState_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  0,  7,  0,  0, 10, 11
    };
  static const struct NsAppLinkRPCV2::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 17 "../src/ALRPCObjectsImpl/V2/AudioStreamingStateMarshaller.gp"
      {"AUDIBLE",0},
      {""}, {""},
#line 18 "../src/ALRPCObjectsImpl/V2/AudioStreamingStateMarshaller.gp"
      {"ATTENUATED",1},
#line 19 "../src/ALRPCObjectsImpl/V2/AudioStreamingStateMarshaller.gp"
      {"NOT_AUDIBLE",2}
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
}