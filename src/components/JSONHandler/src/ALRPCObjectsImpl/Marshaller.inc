/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=../src/ALRPCObjectsImpl/Marshaller.inc ../src/ALRPCObjectsImpl/Marshaller.gp  */
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


#define TOTAL_KEYWORDS 130
#define MIN_WORD_LENGTH 12
#define MAX_WORD_LENGTH 42
#define MIN_HASH_VALUE 15
#define MAX_HASH_VALUE 218
/* maximum key range = 204, duplicates = 0 */

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
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219,  40,  85, 135,   0,   5,
      140,  75, 219, 219, 219, 219, 219,  90, 219, 219,
      219, 120, 219, 125, 219, 219,  55, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 115,   0,   5,   0,
      120,  45,  10, 219, 150,  25,  60, 219, 145,  20,
       25,   5, 105,   0,  15,   0, 100,  20, 155,   5,
      219, 219,  30, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
      219, 219, 219, 219, 219, 219, 219
    };
  return len + asso_values[(unsigned char)str[8]+1] + asso_values[(unsigned char)str[3]];
}

const struct NsAppLinkRPC::Marshaller::localHash *
Marshaller_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0, 15, 16,  0, 13,  0, 15, 16, 22, 23,  0,  0,  0,  0,
      13, 24, 25,  0,  0, 23, 24,  0, 26, 27, 28, 29,  0, 26,
      27, 13, 29, 30, 21, 22, 23, 24, 25, 26, 32, 28, 34, 35,
       0, 37, 38, 24, 25,  0, 27, 28, 34, 35, 21, 22,  0, 24,
      25,  0, 22,  0, 29, 30,  0, 32, 33, 24,  0, 26, 27, 18,
      19, 30, 31,  0, 33, 34,  0, 21, 22,  0, 24, 25,  0, 22,
      23, 34, 35, 31, 37, 38, 34, 25, 26,  0, 28, 29, 30, 31,
       0,  0, 14,  0, 26, 27,  0, 29, 30,  0, 27, 28, 29, 30,
      31, 32,  0, 14,  0, 16, 17,  0, 24, 15, 16, 27, 28, 29,
       0, 31, 32,  0, 14, 15, 16, 17, 18, 19,  0, 21, 22,  0,
      39, 15, 16, 42, 18, 19,  0, 21, 22, 18, 19,  0, 26,  0,
       0, 29,  0,  0, 12, 18, 19,  0,  0, 32,  0, 29,  0,  0,
       0,  0, 24, 25, 26,  0,  0, 29,  0,  0,  0, 28, 29,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0, 17, 18
    };
  static const struct NsAppLinkRPC::Marshaller::localHash wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 90 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"GetDTCs_request",72,&NsAppLinkRPC::Marshaller::mGetDTCs_request},
#line 91 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"GetDTCs_response",73,&NsAppLinkRPC::Marshaller::mGetDTCs_response},
      {""},
#line 43 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"Show_response",25,&NsAppLinkRPC::Marshaller::mShow_response},
      {""},
#line 128 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"Show_v2_request",110,&NsAppLinkRPC::Marshaller::mShow_v2_request},
#line 129 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"Show_v2_response",111,&NsAppLinkRPC::Marshaller::mShow_v2_response},
#line 146 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"UpdateTurnList_request",128,&NsAppLinkRPC::Marshaller::mUpdateTurnList_request},
#line 147 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"UpdateTurnList_response",129,&NsAppLinkRPC::Marshaller::mUpdateTurnList_response},
      {""}, {""}, {""}, {""},
#line 44 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"Speak_request",26,&NsAppLinkRPC::Marshaller::mSpeak_request},
#line 122 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SetDisplayLayout_request",104,&NsAppLinkRPC::Marshaller::mSetDisplayLayout_request},
#line 123 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SetDisplayLayout_response",105,&NsAppLinkRPC::Marshaller::mSetDisplayLayout_response},
      {""}, {""},
#line 50 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SubscribeButton_request",32,&NsAppLinkRPC::Marshaller::mSubscribeButton_request},
#line 51 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SubscribeButton_response",33,&NsAppLinkRPC::Marshaller::mSubscribeButton_response},
      {""},
#line 136 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SubscribeButton_v2_request",118,&NsAppLinkRPC::Marshaller::mSubscribeButton_v2_request},
#line 137 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SubscribeButton_v2_response",119,&NsAppLinkRPC::Marshaller::mSubscribeButton_v2_response},
#line 138 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SubscribeVehicleData_request",120,&NsAppLinkRPC::Marshaller::mSubscribeVehicleData_request},
#line 139 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SubscribeVehicleData_response",121,&NsAppLinkRPC::Marshaller::mSubscribeVehicleData_response},
      {""},
#line 36 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"PerformInteraction_request",18,&NsAppLinkRPC::Marshaller::mPerformInteraction_request},
#line 37 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"PerformInteraction_response",19,&NsAppLinkRPC::Marshaller::mPerformInteraction_response},
#line 40 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"Alert_request",22,&NsAppLinkRPC::Marshaller::mAlert_request},
#line 108 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"PerformInteraction_v2_request",90,&NsAppLinkRPC::Marshaller::mPerformInteraction_v2_request},
#line 109 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"PerformInteraction_v2_response",91,&NsAppLinkRPC::Marshaller::mPerformInteraction_v2_response},
#line 69 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"AlertManeuver_request",51,&NsAppLinkRPC::Marshaller::mAlertManeuver_request},
#line 70 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"AlertManeuver_response",52,&NsAppLinkRPC::Marshaller::mAlertManeuver_response},
#line 130 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"ShowConstantTBT_request",112,&NsAppLinkRPC::Marshaller::mShowConstantTBT_request},
#line 131 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"ShowConstantTBT_response",113,&NsAppLinkRPC::Marshaller::mShowConstantTBT_response},
#line 118 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"ScrollableMessage_request",100,&NsAppLinkRPC::Marshaller::mScrollableMessage_request},
#line 119 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"ScrollableMessage_response",101,&NsAppLinkRPC::Marshaller::mScrollableMessage_response},
#line 62 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnDriverDistraction_notification",44,&NsAppLinkRPC::Marshaller::mOnDriverDistraction},
#line 97 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnAudioPassThru_notification",79,&NsAppLinkRPC::Marshaller::mOnAudioPassThru},
#line 34 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"CreateInteractionChoiceSet_request",16,&NsAppLinkRPC::Marshaller::mCreateInteractionChoiceSet_request},
#line 35 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"CreateInteractionChoiceSet_response",17,&NsAppLinkRPC::Marshaller::mCreateInteractionChoiceSet_response},
      {""},
#line 73 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"CreateInteractionChoiceSet_v2_request",55,&NsAppLinkRPC::Marshaller::mCreateInteractionChoiceSet_v2_request},
#line 74 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"CreateInteractionChoiceSet_v2_response",56,&NsAppLinkRPC::Marshaller::mCreateInteractionChoiceSet_v2_response},
#line 48 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"EncodedSyncPData_request",30,&NsAppLinkRPC::Marshaller::mEncodedSyncPData_request},
#line 49 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"EncodedSyncPData_response",31,&NsAppLinkRPC::Marshaller::mEncodedSyncPData_response},
      {""},
#line 85 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"EncodedSyncPData_v2_request",67,&NsAppLinkRPC::Marshaller::mEncodedSyncPData_v2_request},
#line 86 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"EncodedSyncPData_v2_response",68,&NsAppLinkRPC::Marshaller::mEncodedSyncPData_v2_response},
#line 71 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"ChangeLanguageRegistration_request",53,&NsAppLinkRPC::Marshaller::mChangeLanguageRegistration_request},
#line 72 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"ChangeLanguageRegistration_response",54,&NsAppLinkRPC::Marshaller::mChangeLanguageRegistration_response},
#line 32 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteSubMenu_request",14,&NsAppLinkRPC::Marshaller::mDeleteSubMenu_request},
#line 33 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteSubMenu_response",15,&NsAppLinkRPC::Marshaller::mDeleteSubMenu_response},
      {""},
#line 81 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteSubMenu_v2_request",63,&NsAppLinkRPC::Marshaller::mDeleteSubMenu_v2_request},
#line 82 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteSubMenu_v2_response",64,&NsAppLinkRPC::Marshaller::mDeleteSubMenu_v2_response},
      {""},
#line 59 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnCommand_notification",41,&NsAppLinkRPC::Marshaller::mOnCommand},
      {""},
#line 24 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"ResetGlobalProperties_request",6,&NsAppLinkRPC::Marshaller::mResetGlobalProperties_request},
#line 25 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"ResetGlobalProperties_response",7,&NsAppLinkRPC::Marshaller::mResetGlobalProperties_response},
      {""},
#line 116 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"ResetGlobalProperties_v2_request",98,&NsAppLinkRPC::Marshaller::mResetGlobalProperties_v2_request},
#line 117 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"ResetGlobalProperties_v2_response",99,&NsAppLinkRPC::Marshaller::mResetGlobalProperties_v2_response},
#line 54 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"GenericResponse_response",36,&NsAppLinkRPC::Marshaller::mGenericResponse_response},
      {""},
#line 105 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnVehicleData_notification",87,&NsAppLinkRPC::Marshaller::mOnVehicleData},
#line 89 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"GenericResponse_v2_response",71,&NsAppLinkRPC::Marshaller::mGenericResponse_v2_response},
#line 77 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteFile_request",59,&NsAppLinkRPC::Marshaller::mDeleteFile_request},
#line 78 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteFile_response",60,&NsAppLinkRPC::Marshaller::mDeleteFile_response},
#line 20 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"UnregisterAppInterface_request",2,&NsAppLinkRPC::Marshaller::mUnregisterAppInterface_request},
#line 21 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"UnregisterAppInterface_response",3,&NsAppLinkRPC::Marshaller::mUnregisterAppInterface_response},
      {""},
#line 140 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"UnregisterAppInterface_v2_request",122,&NsAppLinkRPC::Marshaller::mUnregisterAppInterface_v2_request},
#line 141 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"UnregisterAppInterface_v2_response",123,&NsAppLinkRPC::Marshaller::mUnregisterAppInterface_v2_response},
      {""},
#line 28 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteCommand_request",10,&NsAppLinkRPC::Marshaller::mDeleteCommand_request},
#line 29 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteCommand_response",11,&NsAppLinkRPC::Marshaller::mDeleteCommand_response},
      {""},
#line 75 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteCommand_v2_request",57,&NsAppLinkRPC::Marshaller::mDeleteCommand_v2_request},
#line 76 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteCommand_v2_response",58,&NsAppLinkRPC::Marshaller::mDeleteCommand_v2_response},
      {""},
#line 92 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"GetVehicleData_request",74,&NsAppLinkRPC::Marshaller::mGetVehicleData_request},
#line 93 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"GetVehicleData_response",75,&NsAppLinkRPC::Marshaller::mGetVehicleData_response},
#line 38 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteInteractionChoiceSet_request",20,&NsAppLinkRPC::Marshaller::mDeleteInteractionChoiceSet_request},
#line 39 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteInteractionChoiceSet_response",21,&NsAppLinkRPC::Marshaller::mDeleteInteractionChoiceSet_response},
#line 60 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnEncodedSyncPData_notification",42,&NsAppLinkRPC::Marshaller::mOnEncodedSyncPData},
#line 79 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteInteractionChoiceSet_v2_request",61,&NsAppLinkRPC::Marshaller::mDeleteInteractionChoiceSet_v2_request},
#line 80 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"DeleteInteractionChoiceSet_v2_response",62,&NsAppLinkRPC::Marshaller::mDeleteInteractionChoiceSet_v2_response},
#line 100 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnEncodedSyncPData_v2_notification",82,&NsAppLinkRPC::Marshaller::mOnEncodedSyncPData_v2},
#line 52 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"UnsubscribeButton_request",34,&NsAppLinkRPC::Marshaller::mUnsubscribeButton_request},
#line 53 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"UnsubscribeButton_response",35,&NsAppLinkRPC::Marshaller::mUnsubscribeButton_response},
      {""},
#line 142 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"UnsubscribeButton_v2_request",124,&NsAppLinkRPC::Marshaller::mUnsubscribeButton_v2_request},
#line 143 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"UnsubscribeButton_v2_response",125,&NsAppLinkRPC::Marshaller::mUnsubscribeButton_v2_response},
#line 144 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"UnsubscribeVehicleData_request",126,&NsAppLinkRPC::Marshaller::mUnsubscribeVehicleData_request},
#line 145 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"UnsubscribeVehicleData_response",127,&NsAppLinkRPC::Marshaller::mUnsubscribeVehicleData_response},
      {""}, {""},
#line 45 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"Speak_response",27,&NsAppLinkRPC::Marshaller::mSpeak_response},
      {""},
#line 46 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SetMediaClockTimer_request",28,&NsAppLinkRPC::Marshaller::mSetMediaClockTimer_request},
#line 47 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SetMediaClockTimer_response",29,&NsAppLinkRPC::Marshaller::mSetMediaClockTimer_response},
      {""},
#line 126 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SetMediaClockTimer_v2_request",108,&NsAppLinkRPC::Marshaller::mSetMediaClockTimer_v2_request},
#line 127 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SetMediaClockTimer_v2_response",109,&NsAppLinkRPC::Marshaller::mSetMediaClockTimer_v2_response},
      {""},
#line 22 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SetGlobalProperties_request",4,&NsAppLinkRPC::Marshaller::mSetGlobalProperties_request},
#line 23 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SetGlobalProperties_response",5,&NsAppLinkRPC::Marshaller::mSetGlobalProperties_response},
#line 61 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnTBTClientState_notification",43,&NsAppLinkRPC::Marshaller::mOnTBTClientState},
#line 124 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SetGlobalProperties_v2_request",106,&NsAppLinkRPC::Marshaller::mSetGlobalProperties_v2_request},
#line 125 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SetGlobalProperties_v2_response",107,&NsAppLinkRPC::Marshaller::mSetGlobalProperties_v2_response},
#line 104 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnTBTClientState_v2_notification",86,&NsAppLinkRPC::Marshaller::mOnTBTClientState_v2},
      {""},
#line 41 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"Alert_response",23,&NsAppLinkRPC::Marshaller::mAlert_response},
      {""},
#line 134 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"Speak_v2_request",116,&NsAppLinkRPC::Marshaller::mSpeak_v2_request},
#line 135 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"Speak_v2_response",117,&NsAppLinkRPC::Marshaller::mSpeak_v2_response},
      {""},
#line 55 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnHMIStatus_notification",37,&NsAppLinkRPC::Marshaller::mOnHMIStatus},
#line 112 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"ReadDID_request",94,&NsAppLinkRPC::Marshaller::mReadDID_request},
#line 113 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"ReadDID_response",95,&NsAppLinkRPC::Marshaller::mReadDID_response},
#line 101 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnHMIStatus_v2_notification",83,&NsAppLinkRPC::Marshaller::mOnHMIStatus_v2},
#line 18 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"RegisterAppInterface_request",0,&NsAppLinkRPC::Marshaller::mRegisterAppInterface_request},
#line 19 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"RegisterAppInterface_response",1,&NsAppLinkRPC::Marshaller::mRegisterAppInterface_response},
      {""},
#line 114 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"RegisterAppInterface_v2_request",96,&NsAppLinkRPC::Marshaller::mRegisterAppInterface_v2_request},
#line 115 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"RegisterAppInterface_v2_response",97,&NsAppLinkRPC::Marshaller::mRegisterAppInterface_v2_response},
      {""},
#line 132 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"Slider_request",114,&NsAppLinkRPC::Marshaller::mSlider_request},
#line 133 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"Slider_response",115,&NsAppLinkRPC::Marshaller::mSlider_response},
#line 67 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"Alert_v2_request",49,&NsAppLinkRPC::Marshaller::mAlert_v2_request},
#line 68 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"Alert_v2_response",50,&NsAppLinkRPC::Marshaller::mAlert_v2_response},
#line 30 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"AddSubMenu_request",12,&NsAppLinkRPC::Marshaller::mAddSubMenu_request},
#line 31 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"AddSubMenu_response",13,&NsAppLinkRPC::Marshaller::mAddSubMenu_response},
      {""},
#line 65 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"AddSubMenu_v2_request",47,&NsAppLinkRPC::Marshaller::mAddSubMenu_v2_request},
#line 66 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"AddSubMenu_v2_response",48,&NsAppLinkRPC::Marshaller::mAddSubMenu_v2_response},
      {""},
#line 56 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnAppInterfaceUnregistered_notification",38,&NsAppLinkRPC::Marshaller::mOnAppInterfaceUnregistered},
#line 110 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"PutFile_request",92,&NsAppLinkRPC::Marshaller::mPutFile_request},
#line 111 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"PutFile_response",93,&NsAppLinkRPC::Marshaller::mPutFile_response},
#line 96 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnAppInterfaceUnregistered_v2_notification",78,&NsAppLinkRPC::Marshaller::mOnAppInterfaceUnregistered_v2},
#line 26 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"AddCommand_request",8,&NsAppLinkRPC::Marshaller::mAddCommand_request},
#line 27 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"AddCommand_response",9,&NsAppLinkRPC::Marshaller::mAddCommand_response},
      {""},
#line 63 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"AddCommand_v2_request",45,&NsAppLinkRPC::Marshaller::mAddCommand_v2_request},
#line 64 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"AddCommand_v2_response",46,&NsAppLinkRPC::Marshaller::mAddCommand_v2_response},
#line 120 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SetAppIcon_request",102,&NsAppLinkRPC::Marshaller::mSetAppIcon_request},
#line 121 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"SetAppIcon_response",103,&NsAppLinkRPC::Marshaller::mSetAppIcon_response},
      {""},
#line 58 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnButtonPress_notification",40,&NsAppLinkRPC::Marshaller::mOnButtonPress},
      {""}, {""},
#line 99 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnButtonPress_v2_notification",81,&NsAppLinkRPC::Marshaller::mOnButtonPress_v2},
      {""}, {""},
#line 42 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"Show_request",24,&NsAppLinkRPC::Marshaller::mShow_request},
#line 83 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"DialNumber_request",65,&NsAppLinkRPC::Marshaller::mDialNumber_request},
#line 84 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"DialNumber_response",66,&NsAppLinkRPC::Marshaller::mDialNumber_response},
      {""}, {""},
#line 103 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnPermissionsChange_notification",85,&NsAppLinkRPC::Marshaller::mOnPermissionsChange},
      {""},
#line 102 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnLanguageChange_notification",84,&NsAppLinkRPC::Marshaller::mOnLanguageChange},
      {""}, {""}, {""}, {""},
#line 87 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"EndAudioPassThru_request",69,&NsAppLinkRPC::Marshaller::mEndAudioPassThru_request},
#line 88 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"EndAudioPassThru_response",70,&NsAppLinkRPC::Marshaller::mEndAudioPassThru_response},
#line 57 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnButtonEvent_notification",39,&NsAppLinkRPC::Marshaller::mOnButtonEvent},
      {""}, {""},
#line 98 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"OnButtonEvent_v2_notification",80,&NsAppLinkRPC::Marshaller::mOnButtonEvent_v2},
      {""}, {""}, {""},
#line 106 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"PerformAudioPassThru_request",88,&NsAppLinkRPC::Marshaller::mPerformAudioPassThru_request},
#line 107 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"PerformAudioPassThru_response",89,&NsAppLinkRPC::Marshaller::mPerformAudioPassThru_response},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 94 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"ListFiles_request",76,&NsAppLinkRPC::Marshaller::mListFiles_request},
#line 95 "../src/ALRPCObjectsImpl/Marshaller.gp"
      {"ListFiles_response",77,&NsAppLinkRPC::Marshaller::mListFiles_response}
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
