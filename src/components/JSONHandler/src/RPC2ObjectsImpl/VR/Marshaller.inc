/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=../src/RPC2ObjectsImpl/VR/Marshaller.inc ../src/RPC2ObjectsImpl/VR/Marshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 7
#define MIN_WORD_LENGTH 12
#define MAX_WORD_LENGTH 26
#define MIN_HASH_VALUE 12
#define MAX_HASH_VALUE 26
/* maximum key range = 15, duplicates = 0 */

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
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 13,
       0,  0, 16,  0, 18,  0,  0, 21,  0,  0, 24,  0, 26
    };
  static const struct Marshaller::localHash wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 24 "../src/RPC2ObjectsImpl/VR/Marshaller.gp"
      {"VR.OnCommand",6,&Marshaller::mOnCommand},
#line 18 "../src/RPC2ObjectsImpl/VR/Marshaller.gp"
      {"VR.AddCommand",0,&Marshaller::mAddCommand},
      {""}, {""},
#line 20 "../src/RPC2ObjectsImpl/VR/Marshaller.gp"
      {"VR.DeleteCommand",2,&Marshaller::mDeleteCommand},
      {""},
#line 22 "../src/RPC2ObjectsImpl/VR/Marshaller.gp"
      {"VR.GetCapabilities",4,&Marshaller::mGetCapabilities},
      {""}, {""},
#line 19 "../src/RPC2ObjectsImpl/VR/Marshaller.gp"
      {"VR.AddCommandResponse",1,&Marshaller::mAddCommandResponse},
      {""}, {""},
#line 21 "../src/RPC2ObjectsImpl/VR/Marshaller.gp"
      {"VR.DeleteCommandResponse",3,&Marshaller::mDeleteCommandResponse},
      {""},
#line 23 "../src/RPC2ObjectsImpl/VR/Marshaller.gp"
      {"VR.GetCapabilitiesResponse",5,&Marshaller::mGetCapabilitiesResponse}
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
