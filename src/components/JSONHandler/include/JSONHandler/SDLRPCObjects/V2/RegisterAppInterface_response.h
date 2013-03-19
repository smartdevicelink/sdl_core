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

#ifndef NSSMARTDEVICELINKKRPCV2_REGISTERAPPINTERFACE_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_REGISTERAPPINTERFACE_RESPONSE_INCLUDE

#include <vector>

#include "ButtonCapabilities.h"
#include "DisplayCapabilities.h"
#include "HmiZoneCapabilities.h"
#include "Language.h"
#include "PresetBankCapabilities.h"
#include "SoftButtonCapabilities.h"
#include "SpeechCapabilities.h"
#include "SyncMsgVersion.h"
#include "VehicleType.h"
#include "VrCapabilities.h"
#include "JSONHandler/SDLRPCResponse.h"


/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    RC
*/

namespace NsSmartDeviceLinkRPCV2
{
    ///  The response to registerAppInterface
    class RegisterAppInterface_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        RegisterAppInterface_response(const RegisterAppInterface_response& c);
        RegisterAppInterface_response(void);

        virtual ~RegisterAppInterface_response(void);

        RegisterAppInterface_response& operator =(const RegisterAppInterface_response&);

        bool checkIntegrity(void);

        const SyncMsgVersion* get_syncMsgVersion(void) const;
        const Language* get_language(void) const;
        const Language* get_hmiDisplayLanguage(void) const;
        const DisplayCapabilities* get_displayCapabilities(void) const;
        const std::vector<ButtonCapabilities>* get_buttonCapabilities(void) const;
        const std::vector<SoftButtonCapabilities>* get_softButtonCapabilities(void) const;
        const PresetBankCapabilities* get_presetBankCapabilities(void) const;
        const std::vector<HmiZoneCapabilities>* get_hmiZoneCapabilities(void) const;
        const std::vector<SpeechCapabilities>* get_speechCapabilities(void) const;
        const std::vector<VrCapabilities>* get_vrCapabilities(void) const;
        const VehicleType* get_vehicleType(void) const;

        void reset_syncMsgVersion(void);
        bool set_syncMsgVersion(const SyncMsgVersion& syncMsgVersion_);
        void reset_language(void);
        bool set_language(const Language& language_);
        void reset_hmiDisplayLanguage(void);
        bool set_hmiDisplayLanguage(const Language& hmiDisplayLanguage_);
        void reset_displayCapabilities(void);
        bool set_displayCapabilities(const DisplayCapabilities& displayCapabilities_);
        void reset_buttonCapabilities(void);
        bool set_buttonCapabilities(const std::vector<ButtonCapabilities>& buttonCapabilities_);
        void reset_softButtonCapabilities(void);
        bool set_softButtonCapabilities(const std::vector<SoftButtonCapabilities>& softButtonCapabilities_);
        void reset_presetBankCapabilities(void);
        bool set_presetBankCapabilities(const PresetBankCapabilities& presetBankCapabilities_);
        void reset_hmiZoneCapabilities(void);
        bool set_hmiZoneCapabilities(const std::vector<HmiZoneCapabilities>& hmiZoneCapabilities_);
        void reset_speechCapabilities(void);
        bool set_speechCapabilities(const std::vector<SpeechCapabilities>& speechCapabilities_);
        void reset_vrCapabilities(void);
        bool set_vrCapabilities(const std::vector<VrCapabilities>& vrCapabilities_);
        void reset_vehicleType(void);
        bool set_vehicleType(const VehicleType& vehicleType_);

    private:
        friend class RegisterAppInterface_responseMarshaller;

        ///  See SyncMsgVersion
        SyncMsgVersion* syncMsgVersion;

        ///  The currently active VR+TTS language on Sync. See "Language" for options.
        Language* language;

        ///  The currently active display language on Sync. See "Language" for       options.
        Language* hmiDisplayLanguage;

        ///  See DisplayCapabilities
        DisplayCapabilities* displayCapabilities;

        ///  See ButtonCapabilities
        std::vector<ButtonCapabilities>* buttonCapabilities;  //!<   [%s..%s]

        /**
             If returned, the platform supports on-screen SoftButtons.
             See SoftButtonCapabilities
        */
        std::vector<SoftButtonCapabilities>* softButtonCapabilities;  //!<   [%s..%s]

        /**
             If returned, the platform supports custom on-screen Presets.
             See PresetBankCapabilities
        */
        PresetBankCapabilities* presetBankCapabilities;

        ///  See HmiZoneCapabilities
        std::vector<HmiZoneCapabilities>* hmiZoneCapabilities;    //!<   [%s..%s]

        ///  See SpeechCapabilities
        std::vector<SpeechCapabilities>* speechCapabilities;  //!<   [%s..%s]

        ///  See VrCapabilities
        std::vector<VrCapabilities>* vrCapabilities;  //!<   [%s..%s]

        ///  Specifies the vehicle's type. See VehicleType.
        VehicleType* vehicleType;
    };
}

#endif
