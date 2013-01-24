/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=../src/ALRPCObjectsImpl/V1/GlobalPropertyMarshaller.inc ../src/ALRPCObjectsImpl/V1/GlobalPropertyMarshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 2
#define MIN_WORD_LENGTH 10
#define MAX_WORD_LENGTH 13
#define MIN_HASH_VALUE 10
#define MAX_HASH_VALUE 13
/* maximum key range = 4, duplicates = 0 */
namespace NsAppLinkRPC
{
class GlobalProperty_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsAppLinkRPC::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline /*ARGSUSED*/
unsigned int
GlobalProperty_intHash::hash (register const char *str, register unsigned int len)
{
  return len;
}

const struct NsAppLinkRPC::PerfectHashTable *
GlobalProperty_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 10,  0,  0, 13
    };
  static const struct NsAppLinkRPC::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 17 "../src/ALRPCObjectsImpl/V1/GlobalPropertyMarshaller.gp"
      {"HELPPROMPT",0},
      {""}, {""},
#line 18 "../src/ALRPCObjectsImpl/V1/GlobalPropertyMarshaller.gp"
      {"TIMEOUTPROMPT",1}
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
