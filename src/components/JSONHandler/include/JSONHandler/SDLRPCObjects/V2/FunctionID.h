//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef NSSMARTDEVICELINKKRPCV2_FUNCTIONID_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_FUNCTIONID_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{

/**
     Enumeration linking function names with function IDs in WiPro protocol.
     Assumes enumeration starts at value 0.
*/

  class FunctionID
  {
  public:
    enum FunctionIDInternal
    {
      INVALID_ENUM=-1,
      RESERVED=0,
      RegisterAppInterfaceID=1,
      UnregisterAppInterfaceID=2,
      SetGlobalPropertiesID=3,
      ResetGlobalPropertiesID=4,
      AddCommandID=5,
      DeleteCommandID=6,
      AddSubMenuID=7,
      DeleteSubMenuID=8,
      CreateInteractionChoiceSetID=9,
      PerformInteractionID=10,
      DeleteInteractionChoiceSetID=11,
      AlertID=12,
      ShowID=13,
      SpeakID=14,
      SetMediaClockTimerID=15,
      EncodedSyncPDataID=16,
      DialNumberID=17,
      PerformAudioPassThruID=18,
      EndAudioPassThruID=19,
      SubscribeButtonID=20,
      UnsubscribeButtonID=21,
      SubscribeVehicleDataID=22,
      UnsubscribeVehicleDataID=23,
      GetVehicleDataID=24,
      ReadDIDID=25,
      GetDTCsID=26,
      ScrollableMessageID=27,
      SliderID=28,
      ShowConstantTBTID=29,
      AlertManeuverID=30,
      UpdateTurnListID=31,
      ChangeRegistrationID=32,
      GenericResponseID=33,
      PutFileID=34,
      DeleteFileID=35,
      ListFilesID=36,
      SetAppIconID=37,
      SetDisplayLayoutID=38,
      OnHMIStatusID=39,
      OnAppInterfaceUnregisteredID=40,
      OnButtonEventID=41,
      OnButtonPressID=42,
      OnVehicleDataID=43,
      OnCommandID=44,
      OnEncodedSyncPDataID=45,
      OnTBTClientStateID=46,
      OnDriverDistractionID=47,
      OnPermissionsChangeID=48,
      OnAudioPassThruID=49,
      OnLanguageChangeID=50
    };
  
    FunctionID() : mInternal(INVALID_ENUM)				{}
    FunctionID(FunctionIDInternal e) : mInternal(e)		{}
  
    FunctionIDInternal get(void) const	{ return mInternal; }
    void set(FunctionIDInternal e)		{ mInternal=e; }
  
  private:
    FunctionIDInternal mInternal;
    friend class FunctionIDMarshaller;
  };
  
}

#endif
