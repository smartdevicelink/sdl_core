/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=gen/deep/pimpl/next/TriggerSourceMarshaller.inc gen/deep/pimpl/next/TriggerSourceMarshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 2
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 4
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 4
/* maximum key range = 3, duplicates = 0 */

class TriggerSource_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct AppLinkRPC::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline /*ARGSUSED*/
unsigned int
TriggerSource_intHash::hash (register const char *str, register unsigned int len)
{
  return len;
}

const struct AppLinkRPC::PerfectHashTable *
TriggerSource_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  2,  0,  4
    };
  static const struct AppLinkRPC::PerfectHashTable wordlist[] =
    {
      {""}, {""},
#line 18 "gen/deep/pimpl/next/TriggerSourceMarshaller.gp"
      {"VR",1},
      {""},
#line 17 "gen/deep/pimpl/next/TriggerSourceMarshaller.gp"
      {"MENU",0}
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
