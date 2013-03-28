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

#ifndef NSSMARTDEVICELINKKRPC_REGISTERAPPINTERFACE_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPC_REGISTERAPPINTERFACE_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "ButtonCapabilities.h"
#include "DisplayCapabilities.h"
#include "HmiZoneCapabilities.h"
#include "Language.h"
#include "SpeechCapabilities.h"
#include "SyncMsgVersion.h"
#include "VrCapabilities.h"
#include "JSONHandler/SDLRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   1.2
  date      2011-05-17
  generated at  Thu Jan 24 06:36:21 2013
  source stamp  Thu Jan 24 06:35:34 2013
  author    RC
*/

namespace NsSmartDeviceLinkRPC
{
    ///  The response to registerAppInterface
    class RegisterAppInterface_response : public SDLRPCResponse
    {
    public:
        RegisterAppInterface_response(const RegisterAppInterface_response& c);
        RegisterAppInterface_response(void);

        virtual ~RegisterAppInterface_response(void);

        RegisterAppInterface_response& operator =(const RegisterAppInterface_response&);

        bool checkIntegrity(void);

        const SyncMsgVersion* get_syncMsgVersion(void) const;
        const std::string* get_autoActivateID(void) const;
        const Language* get_language(void) const;
        const DisplayCapabilities* get_displayCapabilities(void) const;
        const std::vector<ButtonCapabilities>* get_buttonCapabilities(void) const;
        const std::vector<HmiZoneCapabilities>* get_hmiZoneCapabilities(void) const;
        const std::vector<SpeechCapabilities>* get_speechCapabilities(void) const;
        const std::vector<VrCapabilities>* get_vrCapabilities(void) const;

        void reset_syncMsgVersion(void);
        bool set_syncMsgVersion(const SyncMsgVersion& syncMsgVersion_);
        void reset_autoActivateID(void);
        bool set_autoActivateID(const std::string& autoActivateID_);
        void reset_language(void);
        bool set_language(const Language& language_);
        void reset_displayCapabilities(void);
        bool set_displayCapabilities(const DisplayCapabilities& displayCapabilities_);
        void reset_buttonCapabilities(void);
        bool set_buttonCapabilities(const std::vector<ButtonCapabilities>& buttonCapabilities_);
        void reset_hmiZoneCapabilities(void);
        bool set_hmiZoneCapabilities(const std::vector<HmiZoneCapabilities>& hmiZoneCapabilities_);
        void reset_speechCapabilities(void);
        bool set_speechCapabilities(const std::vector<SpeechCapabilities>& speechCapabilities_);
        void reset_vrCapabilities(void);
        bool set_vrCapabilities(const std::vector<VrCapabilities>& vrCapabilities_);

    private:
        friend class RegisterAppInterface_responseMarshaller;

        ///  See SyncMsgVersion
        SyncMsgVersion* syncMsgVersion;

        ///  Provides an id that the app may use on a subsequent registerAppInterface to
        ///  allow for auto activation (for example after an ignition cycle).
        std::string* autoActivateID;  //!< (16)

        ///  The currently active language on Sync. See "Language" for options.
        Language* language;

        ///  See DisplayCapabilities
        DisplayCapabilities* displayCapabilities;

        ///  See ButtonCapabilities
        std::vector<ButtonCapabilities>* buttonCapabilities;  //!<   [%s..%s]

        ///  See HmiZoneCapabilities
        std::vector<HmiZoneCapabilities>* hmiZoneCapabilities;    //!<   [%s..%s]

        ///  See SpeechCapabilities
        std::vector<SpeechCapabilities>* speechCapabilities;  //!<   [%s..%s]

        ///  See VrCapabilities
        std::vector<VrCapabilities>* vrCapabilities;  //!<   [%s..%s]
    };
}

#endif
