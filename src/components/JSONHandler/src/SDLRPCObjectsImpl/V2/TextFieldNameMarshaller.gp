%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name TextFieldName_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsSmartDeviceLinkRPCV2::PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
mainField1,0
mainField2,1
mainField3,2
mainField4,3
statusBar,4
mediaClock,5
mediaTrack,6
alertText1,7
alertText2,8
alertText3,9
scrollableMessageBody,10
initialInteractionText,11
navigationText1,12
navigationText2,13
ETA,14
totalDistance,15
audioPassThruDisplayText1,16
audioPassThruDisplayText2,17
sliderHeader,18
sliderFooter,19
