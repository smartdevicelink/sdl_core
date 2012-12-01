/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=../src/ALRPCObjectsImpl/PRNDLMarshaller.inc ../src/ALRPCObjectsImpl/PRNDLMarshaller.gp  */
/* Computed positions: -k'1,$' */

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


#define TOTAL_KEYWORDS 14
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 16
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 35
/* maximum key range = 32, duplicates = 0 */

class PRNDL_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsAppLinkRPC::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline unsigned int
PRNDL_intHash::hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      14,  9,  4, 15, 10, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36,  0, 10,
       5, 36, 36, 36, 36,  0,  0, 36,  5, 36,
       0, 36,  0,  0, 10, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
      36, 36, 36, 36, 36, 36
    };
  return len + asso_values[(unsigned char)str[len - 1]] + asso_values[(unsigned char)str[0]];
}

const struct NsAppLinkRPC::PerfectHashTable *
PRNDL_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  4,  0,  0,  7,  0,  0,  0,  0,  7,  0,
       0, 15, 16,  7,  0,  0, 15, 16,  0,  0, 15, 15,  0,  0,
       0, 15, 15,  0,  0,  0, 15, 15
    };
  static const struct NsAppLinkRPC::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 17 "../src/ALRPCObjectsImpl/PRNDLMarshaller.gp"
      {"PARK",0},
      {""}, {""},
#line 20 "../src/ALRPCObjectsImpl/PRNDLMarshaller.gp"
      {"LOWGEAR",3},
      {""}, {""}, {""}, {""},
#line 19 "../src/ALRPCObjectsImpl/PRNDLMarshaller.gp"
      {"NEUTRAL",2},
      {""}, {""},
#line 30 "../src/ALRPCObjectsImpl/PRNDLMarshaller.gp"
      {"SIXTH_GEAR_HOLD",13},
#line 26 "../src/ALRPCObjectsImpl/PRNDLMarshaller.gp"
      {"SECOND_GEAR_HOLD",9},
#line 18 "../src/ALRPCObjectsImpl/PRNDLMarshaller.gp"
      {"REVERSE",1},
      {""}, {""},
#line 29 "../src/ALRPCObjectsImpl/PRNDLMarshaller.gp"
      {"FIFTH_GEAR_HOLD",12},
#line 28 "../src/ALRPCObjectsImpl/PRNDLMarshaller.gp"
      {"FOURTH_GEAR_HOLD",11},
      {""}, {""},
#line 23 "../src/ALRPCObjectsImpl/PRNDLMarshaller.gp"
      {"FORWARD_DRIVE_4",6},
#line 27 "../src/ALRPCObjectsImpl/PRNDLMarshaller.gp"
      {"THIRD_GEAR_HOLD",10},
      {""}, {""}, {""},
#line 22 "../src/ALRPCObjectsImpl/PRNDLMarshaller.gp"
      {"FORWARD_DRIVE_3",5},
#line 25 "../src/ALRPCObjectsImpl/PRNDLMarshaller.gp"
      {"FORWARD_DRIVE_6",8},
      {""}, {""}, {""},
#line 21 "../src/ALRPCObjectsImpl/PRNDLMarshaller.gp"
      {"FORWARD_DRIVE_2",4},
#line 24 "../src/ALRPCObjectsImpl/PRNDLMarshaller.gp"
      {"FORWARD_DRIVE_5",7}
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
