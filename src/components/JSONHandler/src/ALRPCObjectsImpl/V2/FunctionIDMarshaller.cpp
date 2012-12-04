#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/V2/FunctionID.h"
#include "FunctionIDMarshaller.h"
#include "FunctionIDMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


const FunctionID::FunctionIDInternal FunctionIDMarshaller::getIndex(const char* s)
{
  if(!s)
    return FunctionID::INVALID_ENUM;
  const struct PerfectHashTable* p=FunctionID_intHash::getPointer(s,strlen(s));
  return p ? static_cast<FunctionID::FunctionIDInternal>(p->idx) : FunctionID::INVALID_ENUM;
}


bool FunctionIDMarshaller::fromJSON(const Json::Value& s,FunctionID& e)
{
  e.mInternal=FunctionID::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=FunctionID::INVALID_ENUM);
}


Json::Value FunctionIDMarshaller::toJSON(const FunctionID& e)
{
  if(e.mInternal==FunctionID::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool FunctionIDMarshaller::fromString(const std::string& s,FunctionID& e)
{
  e.mInternal=FunctionID::INVALID_ENUM;
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(fromJSON(json,e))  return true;
  }
  catch(...)
  {
    return false;
  }
  return false;
}

const std::string FunctionIDMarshaller::toString(const FunctionID& e)
{
  Json::FastWriter writer;
  return e.mInternal==FunctionID::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable FunctionIDMarshaller::mHashTable[51]=
{
  {"RESERVED",0},
  {"RegisterAppInterfaceID",1},
  {"UnregisterAppInterfaceID",2},
  {"SetGlobalPropertiesID",3},
  {"ResetGlobalPropertiesID",4},
  {"AddCommandID",5},
  {"DeleteCommandID",6},
  {"AddSubMenuID",7},
  {"DeleteSubMenuID",8},
  {"CreateInteractionChoiceSetID",9},
  {"PerformInteractionID",10},
  {"DeleteInteractionChoiceSetID",11},
  {"AlertID",12},
  {"ShowID",13},
  {"SpeakID",14},
  {"SetMediaClockTimerID",15},
  {"EncodedSyncPDataID",16},
  {"DialNumberID",17},
  {"PerformAudioPassThruID",18},
  {"EndAudioPassThruID",19},
  {"SubscribeButtonID",20},
  {"UnsubscribeButtonID",21},
  {"SubscribeVehicleDataID",22},
  {"UnsubscribeVehicleDataID",23},
  {"GetVehicleDataID",24},
  {"ReadDIDID",25},
  {"GetDTCsID",26},
  {"ScrollableMessageID",27},
  {"SliderID",28},
  {"ShowConstantTBTID",29},
  {"AlertManeuverID",30},
  {"UpdateTurnListID",31},
  {"ChangeLanguageRegistrationID",32},
  {"GenericResponseID",33},
  {"PutFileID",34},
  {"DeleteFileID",35},
  {"ListFilesID",36},
  {"SetAppIconID",37},
  {"SetDisplayLayoutID",38},
  {"OnHMIStatusID",39},
  {"OnAppInterfaceUnregisteredID",40},
  {"OnButtonEventID",41},
  {"OnButtonPressID",42},
  {"OnVehicleDataID",43},
  {"OnCommandID",44},
  {"OnEncodedSyncPDataID",45},
  {"OnTBTClientStateID",46},
  {"OnDriverDistractionID",47},
  {"OnPermissionsChangeID",48},
  {"OnAudioPassThruID",49},
  {"OnLanguageChangeID",50}
};
