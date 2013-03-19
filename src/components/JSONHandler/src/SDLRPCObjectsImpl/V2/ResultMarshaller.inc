/**
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.inc ./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp  */
/* Computed positions: -k'1,13' */

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


#define TOTAL_KEYWORDS 30
#define MIN_WORD_LENGTH 5
#define MAX_WORD_LENGTH 30
#define MIN_HASH_VALUE 5
#define MAX_HASH_VALUE 50
/* maximum key range = 46, duplicates = 0 */

namespace NsSmartDeviceLinkRPCV2 {

class Result_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsSmartDeviceLinkRPCV2::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline unsigned int
Result_intHash::hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 20,  5, 10,  5, 23,
       0, 20, 51,  0, 51, 51,  0, 15,  0,  5,
      51, 51,  0, 10,  0,  0,  5,  5, 51,  0,
      51, 51, 51, 51, 51,  0, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[12]];
      /*FALLTHROUGH*/
      case 12:
      case 11:
      case 10:
      case 9:
      case 8:
      case 7:
      case 6:
      case 5:
      case 4:
      case 3:
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

const struct NsSmartDeviceLinkRPCV2::PerfectHashTable *
Result_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  5,  6,  7,  8,  9, 10,  0, 12,  8,
      14, 10,  6,  7, 13, 19, 15, 21, 12, 18, 19,  0,  0,  7,
      18, 24, 25, 26,  9, 13, 14,  0,  0,  0,  0, 14,  0,  0,
       0,  0,  0,  0, 26,  0,  0,  0, 30
    };
  static const struct NsSmartDeviceLinkRPCV2::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""},
#line 46 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"RETRY",29},
#line 29 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"IN_USE",12},
#line 33 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"IGNORED",16},
#line 31 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"REJECTED",14},
#line 41 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"TIMED_OUT",24},
#line 22 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"INVALID_ID",5},
      {""},
#line 18 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"INVALID_DATA",1},
#line 45 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"WARNINGS",28},
#line 35 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"FILE_NOT_FOUND",18},
#line 39 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"DISALLOWED",22},
#line 43 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"CLOSED",26},
#line 17 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"SUCCESS",0},
#line 20 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"OUT_OF_MEMORY",3},
#line 19 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"UNSUPPORTED_REQUEST",2},
#line 40 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"USER_DISALLOWED",23},
#line 24 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"TOO_MANY_APPLICATIONS",7},
#line 42 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"CANCEL_ROUTE",25},
#line 34 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"UNSUPPORTED_BUTTON",17},
#line 26 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"UNSUPPORTED_VERSION",9},
      {""}, {""},
#line 32 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"ABORTED",15},
#line 30 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"SUBSCRIBED_ALREADY",13},
#line 37 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"VEHICLE_DATA_NOT_ALLOWED",20},
#line 21 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"TOO_MANY_PENDING_REQUESTS",4},
#line 38 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"VEHICLE_DATA_NOT_AVAILABLE",21},
#line 44 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"ENCRYPTED",27},
#line 36 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"GENERIC_ERROR",19},
#line 23 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"DUPLICATE_NAME",6},
      {""}, {""}, {""}, {""},
#line 27 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"WRONG_LANGUAGE",10},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 28 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"APPLICATION_NOT_REGISTERED",11},
      {""}, {""}, {""},
#line 25 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/ResultMarshaller.gp"
      {"APPLICATION_REGISTERED_ALREADY",8}
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
