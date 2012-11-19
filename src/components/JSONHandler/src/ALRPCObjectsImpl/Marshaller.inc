/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=./src/ALRPCObjectsImpl/Marshaller.inc ./src/ALRPCObjectsImpl/Marshaller.gp  */
/* Computed positions: -k'1,10' */

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


#define TOTAL_KEYWORDS 45
#define MIN_WORD_LENGTH 12
#define MAX_WORD_LENGTH 39
#define MIN_HASH_VALUE 12
#define MAX_HASH_VALUE 79
/* maximum key range = 68, duplicates = 0 */

class Marshaller_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsAppLinkRPC::Marshaller::localHash *getPointer (const char *str, unsigned int len);
};

inline unsigned int
Marshaller_intHash::hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80,  5, 50, 35,  0, 25,
      80, 30, 80, 80, 80, 80, 80, 40, 80,  5,
       0, 80, 15,  0, 80, 10, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 15, 80, 15, 20, 80,
      15,  0, 80, 80, 80,  0, 80, 80, 20,  0,
      15, 20,  0, 80, 10, 25,  5,  0, 35, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80
    };
  return len + asso_values[(unsigned char)str[9]] + asso_values[(unsigned char)str[0]];
}

const struct NsAppLinkRPC::Marshaller::localHash *
Marshaller_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 13,
      14,  0,  0,  0, 13, 14,  0, 21, 22, 18, 19,  0,  0, 27,
      28, 24,  0, 26, 27, 13, 34, 35, 31, 32, 18, 19,  0, 26,
      22, 28, 29,  0, 26, 27,  0, 29, 30, 31,  0,  0, 39, 25,
      26,  0,  0, 29, 30, 21, 22,  0, 24, 25, 26,  0,  0, 34,
      35,  0,  0, 23, 24,  0,  0,  0,  0, 24
    };
  static const struct NsAppLinkRPC::Marshaller::localHash wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 42 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"Show_request",24,&NsAppLinkRPC::Marshaller::mShow_request},
#line 44 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"Speak_request",26,&NsAppLinkRPC::Marshaller::mSpeak_request},
#line 45 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"Speak_response",27,&NsAppLinkRPC::Marshaller::mSpeak_response},
      {""}, {""}, {""},
#line 40 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"Alert_request",22,&NsAppLinkRPC::Marshaller::mAlert_request},
#line 41 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"Alert_response",23,&NsAppLinkRPC::Marshaller::mAlert_response},
      {""},
#line 28 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteCommand_request",10,&NsAppLinkRPC::Marshaller::mDeleteCommand_request},
#line 29 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteCommand_response",11,&NsAppLinkRPC::Marshaller::mDeleteCommand_response},
#line 30 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"AddSubMenu_request",12,&NsAppLinkRPC::Marshaller::mAddSubMenu_request},
#line 31 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"AddSubMenu_response",13,&NsAppLinkRPC::Marshaller::mAddSubMenu_response},
      {""}, {""},
#line 22 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"SetGlobalProperties_request",4,&NsAppLinkRPC::Marshaller::mSetGlobalProperties_request},
#line 23 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"SetGlobalProperties_response",5,&NsAppLinkRPC::Marshaller::mSetGlobalProperties_response},
#line 55 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnHMIStatus_notification",37,&NsAppLinkRPC::Marshaller::mOnHMIStatus},
      {""},
#line 36 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"PerformInteraction_request",18,&NsAppLinkRPC::Marshaller::mPerformInteraction_request},
#line 37 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"PerformInteraction_response",19,&NsAppLinkRPC::Marshaller::mPerformInteraction_response},
#line 43 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"Show_response",25,&NsAppLinkRPC::Marshaller::mShow_response},
#line 38 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteInteractionChoiceSet_request",20,&NsAppLinkRPC::Marshaller::mDeleteInteractionChoiceSet_request},
#line 39 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteInteractionChoiceSet_response",21,&NsAppLinkRPC::Marshaller::mDeleteInteractionChoiceSet_response},
#line 60 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnEncodedSyncPData_notification",42,&NsAppLinkRPC::Marshaller::mOnEncodedSyncPData},
#line 62 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnDriverDistraction_notification",44,&NsAppLinkRPC::Marshaller::mOnDriverDistraction},
#line 26 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"AddCommand_request",8,&NsAppLinkRPC::Marshaller::mAddCommand_request},
#line 27 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"AddCommand_response",9,&NsAppLinkRPC::Marshaller::mAddCommand_response},
      {""},
#line 58 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnButtonPress_notification",40,&NsAppLinkRPC::Marshaller::mOnButtonPress},
#line 59 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnCommand_notification",41,&NsAppLinkRPC::Marshaller::mOnCommand},
#line 18 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"RegisterAppInterface_request",0,&NsAppLinkRPC::Marshaller::mRegisterAppInterface_request},
#line 19 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"RegisterAppInterface_response",1,&NsAppLinkRPC::Marshaller::mRegisterAppInterface_response},
      {""},
#line 46 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"SetMediaClockTimer_request",28,&NsAppLinkRPC::Marshaller::mSetMediaClockTimer_request},
#line 47 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"SetMediaClockTimer_response",29,&NsAppLinkRPC::Marshaller::mSetMediaClockTimer_response},
      {""},
#line 61 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnTBTClientState_notification",43,&NsAppLinkRPC::Marshaller::mOnTBTClientState},
#line 20 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"UnregisterAppInterface_request",2,&NsAppLinkRPC::Marshaller::mUnregisterAppInterface_request},
#line 21 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"UnregisterAppInterface_response",3,&NsAppLinkRPC::Marshaller::mUnregisterAppInterface_response},
      {""}, {""},
#line 56 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnAppInterfaceUnregistered_notification",38,&NsAppLinkRPC::Marshaller::mOnAppInterfaceUnregistered},
#line 52 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"UnsubscribeButton_request",34,&NsAppLinkRPC::Marshaller::mUnsubscribeButton_request},
#line 53 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"UnsubscribeButton_response",35,&NsAppLinkRPC::Marshaller::mUnsubscribeButton_response},
      {""}, {""},
#line 24 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"ResetGlobalProperties_request",6,&NsAppLinkRPC::Marshaller::mResetGlobalProperties_request},
#line 25 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"ResetGlobalProperties_response",7,&NsAppLinkRPC::Marshaller::mResetGlobalProperties_response},
#line 32 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteSubMenu_request",14,&NsAppLinkRPC::Marshaller::mDeleteSubMenu_request},
#line 33 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteSubMenu_response",15,&NsAppLinkRPC::Marshaller::mDeleteSubMenu_response},
      {""},
#line 48 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"EncodedSyncPData_request",30,&NsAppLinkRPC::Marshaller::mEncodedSyncPData_request},
#line 49 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"EncodedSyncPData_response",31,&NsAppLinkRPC::Marshaller::mEncodedSyncPData_response},
#line 57 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnButtonEvent_notification",39,&NsAppLinkRPC::Marshaller::mOnButtonEvent},
      {""}, {""},
#line 34 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"CreateInteractionChoiceSet_request",16,&NsAppLinkRPC::Marshaller::mCreateInteractionChoiceSet_request},
#line 35 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"CreateInteractionChoiceSet_response",17,&NsAppLinkRPC::Marshaller::mCreateInteractionChoiceSet_response},
      {""}, {""},
#line 50 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"SubscribeButton_request",32,&NsAppLinkRPC::Marshaller::mSubscribeButton_request},
#line 51 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"SubscribeButton_response",33,&NsAppLinkRPC::Marshaller::mSubscribeButton_response},
      {""}, {""}, {""}, {""},
#line 54 "./src/ALRPCObjectsImpl/Marshaller.gp"
      {"GenericResponse_response",36,&NsAppLinkRPC::Marshaller::mGenericResponse_response}
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
