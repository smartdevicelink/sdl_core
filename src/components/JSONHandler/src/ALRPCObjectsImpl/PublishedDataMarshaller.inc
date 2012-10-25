/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=gen/deep/pimpl/next/PublishedDataMarshaller.inc gen/deep/pimpl/next/PublishedDataMarshaller.gp  */
/* Computed positions: -k'3,12' */

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


#define TOTAL_KEYWORDS 17
#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH 22
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 33
/* maximum key range = 31, duplicates = 0 */

class PublishedData_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline unsigned int
PublishedData_intHash::hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 15, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34,  5, 34, 10, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 15, 34, 34,
      34,  5, 34,  5, 34,  0, 34, 34, 34, 20,
       0,  0,  5, 34,  0, 15,  0, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[11]];
      /*FALLTHROUGH*/
      case 11:
      case 10:
      case 9:
      case 8:
      case 7:
      case 6:
      case 5:
      case 4:
      case 3:
        hval += asso_values[(unsigned char)str[2]];
        break;
    }
  return hval;
}

const struct PerfectHashTable *
PublishedData_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  3,  0,  5,  0,  0,  8,  0,  5,  0, 12, 13,
       9,  0,  0, 12,  3, 14,  0,  0, 12,  3, 19,  0,  0, 22,
      18, 14,  0,  0,  0, 18
    };
  static const struct PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""},
#line 33 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"vin",16},
      {""},
#line 28 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"prndl",11},
      {""}, {""},
#line 27 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"odometer",10},
      {""},
#line 30 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"speed",13},
      {""},
#line 32 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"tripOdometer",15},
#line 25 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"genericbinary",8},
#line 24 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"fuelLevel",7},
      {""}, {""},
#line 31 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"tirePressure",14},
#line 26 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"gps",9},
#line 17 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"avgFuelEconomy",0},
      {""}, {""},
#line 22 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"engineTorque",5},
#line 29 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"rpm",12},
#line 23 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"externalTemperature",6},
      {""}, {""},
#line 19 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"batteryPackTemperature",2},
#line 20 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"batteryPackVoltage",3},
#line 21 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"batteryVoltage",4},
      {""}, {""}, {""},
#line 18 "gen/deep/pimpl/next/PublishedDataMarshaller.gp"
      {"batteryPackCurrent",1}
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
