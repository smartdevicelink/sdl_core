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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_CAPABILITIES_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_CAPABILITIES_H_

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
class CSmartObject;
}
}

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

namespace application_manager {

struct HMICapabilities {
    HMICapabilities();
    virtual ~HMICapabilities();

    /*
     * @brief Retrieves if mixing audio is supported by HMI
     * (ie recording TTS command and playing audio)
     *
     * @return Current state of the mixing audio flag
     */
    bool attenuated_supported() const;

    /*
     * @brief Sets state for mixing audio
     *
     * @param state New state to be set
     */
    void set_attenuated_supported(bool state);

    /*
     * @brief Retrieves UI supported languages
     *
     * @return Currently supported UI languages
     */
    inline const smart_objects::CSmartObject*
    ui_supported_languages() const {
      return ui_supported_languages_;
    }

    /*
     * @brief Sets supported UI languages
     *
     * @param supported_languages Supported UI languages
     */
    void set_ui_supported_languages(
      const smart_objects::CSmartObject& supported_languages);

    /*
     * @brief Retrieves TTS  supported languages
     *
     * @return Currently supported TTS languages
     */
    inline const smart_objects::CSmartObject*
    tts_supported_languages() const {
      return tts_supported_languages_;
    }

    /*
     * @brief Sets supported TTS languages
     *
     * @param supported_languages Supported TTS languages
     */
    void set_tts_supported_languages(
      const smart_objects::CSmartObject& supported_languages);

    /*
     * @brief Retrieves VR supported languages
     *
     * @return Currently supported VR languages
     */
    inline const smart_objects::CSmartObject*
    vr_supported_languages() const {
      return vr_supported_languages_;
    }

    /*
     * @brief Sets supported VR languages
     *
     * @param supported_languages Supported VR languages
     */
    void set_vr_supported_languages(
      const smart_objects::CSmartObject& supported_languages);

    /*
     * @brief Retrieves information about the display capabilities
     *
     * @return Currently supported display capabilities
     */
    inline const smart_objects::CSmartObject*
    display_capabilities() const {
      return display_capabilities_;
    }

    /*
     * @brief Sets supported display capabilities
     *
     * @param display_capabilities supported display capabilities
     */
    void set_display_capabilities(
      const smart_objects::CSmartObject& display_capabilities);

    /*
     * @brief Retrieves information about the HMI zone capabilities
     *
     * @return Currently supported HMI zone capabilities
     */
    inline const smart_objects::CSmartObject*
    hmi_zone_capabilities() const {
      return hmi_zone_capabilities_;
    }

    /*
     * @brief Sets supported HMI zone capabilities
     *
     * @param hmi_zone_capabilities supported HMI zone capabilities
     */
    void set_hmi_zone_capabilities(
      const smart_objects::CSmartObject& hmi_zone_capabilities);

    /*
     * @brief Retrieves information about the SoftButton's capabilities
     *
     * @return Currently supported SoftButton's capabilities
     */
    inline const smart_objects::CSmartObject*
    soft_button_capabilities() const {
      return soft_buttons_capabilities_;
    }

    /*
     * @brief Sets supported SoftButton's capabilities
     *
     * @param soft_button_capabilities supported SoftButton's capabilities
     */
    void set_soft_button_capabilities(
      const smart_objects::CSmartObject& soft_button_capabilities);

  protected:
    bool attenuated_supported_;
    smart_objects::CSmartObject* ui_supported_languages_;
    smart_objects::CSmartObject* tts_supported_languages_;
    smart_objects::CSmartObject* vr_supported_languages_;
    smart_objects::CSmartObject* display_capabilities_;
    smart_objects::CSmartObject* hmi_zone_capabilities_;
    smart_objects::CSmartObject* soft_buttons_capabilities_;
    smart_objects::CSmartObject* button_capabilities_;
    smart_objects::CSmartObject* preset_bank_capabilities_;
};
}  //  namespace application_manager

#endif  //  SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_CAPABILITIES_H_
