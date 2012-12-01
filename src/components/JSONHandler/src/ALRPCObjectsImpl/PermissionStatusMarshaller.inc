/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=../src/ALRPCObjectsImpl/PermissionStatusMarshaller.inc ../src/ALRPCObjectsImpl/PermissionStatusMarshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 4
#define MIN_WORD_LENGTH 7
#define MAX_WORD_LENGTH 20
#define MIN_HASH_VALUE 7
#define MAX_HASH_VALUE 20
/* maximum key range = 14, duplicates = 0 */

class PermissionStatus_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsAppLinkRPC::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline /*ARGSUSED*/
unsigned int
PermissionStatus_intHash::hash (register const char *str, register unsigned int len)
{
  return len;
}

const struct NsAppLinkRPC::PerfectHashTable *
PermissionStatus_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  0,  7,  0,  0, 10,  0,  0,  0,
       0, 15,  0,  0,  0,  0, 20
    };
  static const struct NsAppLinkRPC::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 17 "../src/ALRPCObjectsImpl/PermissionStatusMarshaller.gp"
      {"ALLOWED",0},
      {""}, {""},
#line 18 "../src/ALRPCObjectsImpl/PermissionStatusMarshaller.gp"
      {"DISALLOWED",1},
      {""}, {""}, {""}, {""},
#line 19 "../src/ALRPCObjectsImpl/PermissionStatusMarshaller.gp"
      {"USER_DISALLOWED",2},
      {""}, {""}, {""}, {""},
#line 20 "../src/ALRPCObjectsImpl/PermissionStatusMarshaller.gp"
      {"USER_CONSENT_PENDING",3}
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
