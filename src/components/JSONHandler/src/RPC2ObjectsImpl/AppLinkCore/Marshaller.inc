/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=../src/RPC2ObjectsImpl/AppLinkCore/Marshaller.inc ../src/RPC2ObjectsImpl/AppLinkCore/Marshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 4
#define MIN_WORD_LENGTH 23
#define MAX_WORD_LENGTH 31
#define MIN_HASH_VALUE 23
#define MAX_HASH_VALUE 31
/* maximum key range = 9, duplicates = 0 */

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
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0, 23,  0,  0,  0, 27,
       0, 29,  0, 31
    };
  static const struct Marshaller::localHash wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 18 "../src/RPC2ObjectsImpl/AppLinkCore/Marshaller.gp"
      {"AppLinkCore.ActivateApp",0,&Marshaller::mActivateApp},
      {""}, {""}, {""},
#line 20 "../src/RPC2ObjectsImpl/AppLinkCore/Marshaller.gp"
      {"AppLinkCore.OnAppRegistered",2,&Marshaller::mOnAppRegistered},
      {""},
#line 21 "../src/RPC2ObjectsImpl/AppLinkCore/Marshaller.gp"
      {"AppLinkCore.OnAppUnregistered",3,&Marshaller::mOnAppUnregistered},
      {""},
#line 19 "../src/RPC2ObjectsImpl/AppLinkCore/Marshaller.gp"
      {"AppLinkCore.ActivateAppResponse",1,&Marshaller::mActivateAppResponse}
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
