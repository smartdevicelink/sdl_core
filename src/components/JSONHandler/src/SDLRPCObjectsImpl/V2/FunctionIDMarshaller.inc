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
/* Command-line: gperf --output-file=./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.inc ./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp  */
/* Computed positions: -k'4,9' */

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


#define TOTAL_KEYWORDS 51
#define MIN_WORD_LENGTH 6
#define MAX_WORD_LENGTH 28
#define MIN_HASH_VALUE 7
#define MAX_HASH_VALUE 72
/* maximum key range = 66, duplicates = 0 */

class FunctionID_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsSmartDeviceLinkRPCV2::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline unsigned int
FunctionID_intHash::hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 35,  0, 45,  0,  0,
      45, 10, 73, 73, 73, 73, 73, 10, 73, 73,
      73,  0, 73, 35, 73, 73, 10, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73,  0, 25, 30,
       5, 10,  0, 73, 15, 35,  0, 73, 40, 15,
      15, 15, 20, 73,  5,  0,  0,  0, 25, 15,
      73, 73, 25, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[8]+1];
      /*FALLTHROUGH*/
      case 8:
      case 7:
      case 6:
      case 5:
      case 4:
        hval += asso_values[(unsigned char)str[3]];
        break;
    }
  return hval;
}

const struct NsSmartDeviceLinkRPCV2::PerfectHashTable *
FunctionID_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  0,  7,  8,  9,  0, 11,  7,  8,
       9, 15, 16, 17, 18, 19, 15,  6, 22, 13, 24, 15, 21, 17,
      28,  0, 20, 21, 17, 18, 24, 20, 11, 12, 28,  0, 15, 16,
      17, 18,  0, 20, 21, 22, 28, 19, 20,  0, 12, 18,  9, 15,
       0, 22, 18,  0, 15,  0, 12, 23,  0,  0,  0, 12,  0,  0,
       0,  0, 12
    };
  static const struct NsSmartDeviceLinkRPCV2::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 31 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"SpeakID",14},
#line 17 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"RESERVED",0},
#line 43 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"GetDTCsID",26},
      {""},
#line 53 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"ListFilesID",36},
#line 29 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"AlertID",12},
#line 45 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"SliderID",28},
#line 42 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"ReadDIDID",25},
#line 59 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"OnButtonPressID",42},
#line 48 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"UpdateTurnListID",31},
#line 37 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"SubscribeButtonID",20},
#line 63 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"OnTBTClientStateID",46},
#line 38 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"UnsubscribeButtonID",21},
#line 47 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"AlertManeuverID",30},
#line 30 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"ShowID",13},
#line 39 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"SubscribeVehicleDataID",22},
#line 56 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"OnHMIStatusID",39},
#line 40 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"UnsubscribeVehicleDataID",23},
#line 60 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"OnVehicleDataID",43},
#line 64 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"OnDriverDistractionID",47},
#line 50 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"GenericResponseID",33},
#line 26 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"CreateInteractionChoiceSetID",9},
      {""},
#line 32 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"SetMediaClockTimerID",15},
#line 65 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"OnPermissionsChangeID",48},
#line 46 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"ShowConstantTBTID",29},
#line 67 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"OnLanguageChangeID",50},
#line 19 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"UnregisterAppInterfaceID",2},
#line 27 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"PerformInteractionID",10},
#line 61 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"OnCommandID",44},
#line 52 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"DeleteFileID",35},
#line 28 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"DeleteInteractionChoiceSetID",11},
      {""},
#line 23 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"DeleteCommandID",6},
#line 41 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"GetVehicleDataID",24},
#line 66 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"OnAudioPassThruID",49},
#line 55 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"SetDisplayLayoutID",38},
      {""},
#line 62 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"OnEncodedSyncPDataID",45},
#line 20 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"SetGlobalPropertiesID",3},
#line 35 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"PerformAudioPassThruID",18},
#line 57 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"OnAppInterfaceUnregisteredID",40},
#line 44 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"ScrollableMessageID",27},
#line 49 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"ChangeRegistrationID",32},
      {""},
#line 34 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"DialNumberID",17},
#line 36 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"EndAudioPassThruID",19},
#line 51 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"PutFileID",34},
#line 25 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"DeleteSubMenuID",8},
      {""},
#line 18 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"RegisterAppInterfaceID",1},
#line 33 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"EncodedSyncPDataID",16},
      {""},
#line 58 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"OnButtonEventID",41},
      {""},
#line 24 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"AddSubMenuID",7},
#line 21 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"ResetGlobalPropertiesID",4},
      {""}, {""}, {""},
#line 54 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"SetAppIconID",37},
      {""}, {""}, {""}, {""},
#line 22 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/FunctionIDMarshaller.gp"
      {"AddCommandID",5}
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
