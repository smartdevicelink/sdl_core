/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=gen/deep/pimpl/next/AppInterfaceUnregisteredReasonMarshaller.inc gen/deep/pimpl/next/AppInterfaceUnregisteredReasonMarshaller.gp  */
/* Computed positions: -k'1' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif


#define TOTAL_KEYWORDS 10
#define MIN_WORD_LENGTH 9
#define MAX_WORD_LENGTH 31
#define MIN_HASH_VALUE 9
#define MAX_HASH_VALUE 31
/* maximum key range = 23, duplicates = 0 */

class AppInterfaceUnregisteredReason_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline unsigned int
AppInterfaceUnregisteredReason_intHash::hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32,  0, 32,  0, 32,
       5, 32, 32, 10, 32, 32,  0,  0, 32, 32,
      32, 32,  0, 32,  0,  0, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32
    };
  return len + asso_values[(unsigned char)str[0]];
}

const struct PerfectHashTable *
AppInterfaceUnregisteredReason_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  0,  0, 12, 13,
       0, 15, 16, 17,  0,  0,  0, 16, 12,  0,  0,  0,  0,  0,
      28,  0,  0, 31
    };
  static const struct PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 17 "gen/deep/pimpl/next/AppInterfaceUnregisteredReasonMarshaller.gp"
      {"USER_EXIT",0},
      {""}, {""},
#line 25 "gen/deep/pimpl/next/AppInterfaceUnregisteredReasonMarshaller.gp"
      {"MASTER_RESET",8},
#line 19 "gen/deep/pimpl/next/AppInterfaceUnregisteredReasonMarshaller.gp"
      {"BLUETOOTH_OFF",2},
      {""},
#line 24 "gen/deep/pimpl/next/AppInterfaceUnregisteredReasonMarshaller.gp"
      {"LANGUAGE_CHANGE",7},
#line 20 "gen/deep/pimpl/next/AppInterfaceUnregisteredReasonMarshaller.gp"
      {"USB_DISCONNECTED",3},
#line 22 "gen/deep/pimpl/next/AppInterfaceUnregisteredReasonMarshaller.gp"
      {"TOO_MANY_REQUESTS",5},
      {""}, {""}, {""},
#line 26 "gen/deep/pimpl/next/AppInterfaceUnregisteredReasonMarshaller.gp"
      {"FACTORY_DEFAULTS",9},
#line 18 "gen/deep/pimpl/next/AppInterfaceUnregisteredReasonMarshaller.gp"
      {"IGNITION_OFF",1},
      {""}, {""}, {""}, {""}, {""},
#line 23 "gen/deep/pimpl/next/AppInterfaceUnregisteredReasonMarshaller.gp"
      {"DRIVER_DISTRACTION_VIOLATION",6},
      {""}, {""},
#line 21 "gen/deep/pimpl/next/AppInterfaceUnregisteredReasonMarshaller.gp"
      {"REQUEST_WHILE_IN_NONE_HMI_LEVEL",4}
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
