/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=gen/deep/pimpl/next/TirePressureTellTaleMarshaller.inc gen/deep/pimpl/next/TirePressureTellTaleMarshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 3
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 5
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 5
/* maximum key range = 4, duplicates = 0 */

class TirePressureTellTale_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline /*ARGSUSED*/
unsigned int
TirePressureTellTale_intHash::hash (register const char *str, register unsigned int len)
{
  return len;
}

const struct PerfectHashTable *
TirePressureTellTale_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  2,  3,  0,  5
    };
  static const struct PerfectHashTable wordlist[] =
    {
      {""}, {""},
#line 18 "gen/deep/pimpl/next/TirePressureTellTaleMarshaller.gp"
      {"ON",1},
#line 17 "gen/deep/pimpl/next/TirePressureTellTaleMarshaller.gp"
      {"OFF",0},
      {""},
#line 19 "gen/deep/pimpl/next/TirePressureTellTaleMarshaller.gp"
      {"FLASH",2}
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
