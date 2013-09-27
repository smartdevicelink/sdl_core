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

#include "application_manager/hmi_capabilities.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"

namespace application_manager {

HMICapabilities::HMICapabilities()
    : is_vr_ready_response_recieved_(false),
      is_tts_ready_response_recieved_(false),
      is_ui_ready_response_recieved_(false),
      is_navi_ready_response_recieved_(false),
      is_ivi_ready_response_recieved_(false),
      attenuated_supported_(false),
      ui_supported_languages_(NULL),
      tts_supported_languages_(NULL),
      vr_supported_languages_(NULL),
      display_capabilities_(NULL),
      hmi_zone_capabilities_(NULL),
      soft_buttons_capabilities_(NULL),
      button_capabilities_(NULL),
      preset_bank_capabilities_(NULL),
      vr_capabilities_(NULL),
      speech_capabilities_(NULL),
      audio_pass_thru_capabilities_(NULL) {
}

HMICapabilities::~HMICapabilities() {
  delete ui_supported_languages_;
  delete tts_supported_languages_;
  delete vr_supported_languages_;
  delete display_capabilities_;
  delete hmi_zone_capabilities_;
  delete soft_buttons_capabilities_;
  delete button_capabilities_;
  delete preset_bank_capabilities_;
  delete vr_capabilities_;
  delete speech_capabilities_;
  delete audio_pass_thru_capabilities_;
}

bool HMICapabilities::is_vr_cooperating() const {
  return is_vr_cooperating_;
}

void HMICapabilities::set_is_vr_cooperating(bool value) {
  is_vr_cooperating_ = value;
}

bool HMICapabilities::is_tts_cooperating() const {
  return is_tts_cooperating_;
}

void HMICapabilities::set_is_tts_cooperating(bool value) {
  is_tts_cooperating_ = value;
}

bool HMICapabilities::is_ui_cooperating() const {
  return is_ui_cooperating_;
}

void HMICapabilities::set_is_ui_cooperating(bool value) {
  is_ui_cooperating_ = value;
}

bool HMICapabilities::is_navi_cooperating() const {
  return is_navi_cooperating_;
}

void HMICapabilities::set_is_navi_cooperating(bool value) {
  is_navi_cooperating_ = value;
}

bool HMICapabilities::is_ivi_cooperating() const {
  return is_ivi_cooperating_;
}

void HMICapabilities::set_is_ivi_cooperating(bool value) {
  is_ivi_cooperating_ = value;
}

bool HMICapabilities::attenuated_supported() const {
  return attenuated_supported_;
}

void HMICapabilities::set_attenuated_supported(bool state) {
  attenuated_supported_ = state;
}

void HMICapabilities::set_ui_supported_languages(
    const smart_objects::SmartObject& supported_languages) {
  if (ui_supported_languages_) {
    delete ui_supported_languages_;
  }
  ui_supported_languages_ = new smart_objects::SmartObject(supported_languages);
}

void HMICapabilities::set_tts_supported_languages(
    const smart_objects::SmartObject& supported_languages) {
  if (tts_supported_languages_) {
    delete tts_supported_languages_;
  }
  tts_supported_languages_ = new smart_objects::SmartObject(
      supported_languages);
}

void HMICapabilities::set_vr_supported_languages(
    const smart_objects::SmartObject& supported_languages) {
  if (vr_supported_languages_) {
    delete vr_supported_languages_;
  }
  vr_supported_languages_ = new smart_objects::SmartObject(supported_languages);
}

bool HMICapabilities::VerifyImageType(int image_type) {
  if (!display_capabilities_) {
    return false;
  }

  if (display_capabilities_->keyExists(hmi_response::image_capabilities)) {
    const smart_objects::SmartObject& image_caps = display_capabilities_
        ->getElement(hmi_response::image_capabilities);
    for (int i = 0; i < image_caps.length(); ++i) {
      if (image_caps.getElement(i).asInt() == image_type) {
        return true;
      }
    }
  }

  return false;
}

void HMICapabilities::set_display_capabilities(
    const smart_objects::SmartObject& display_capabilities) {
  if (display_capabilities_) {
    delete display_capabilities_;
  }
  display_capabilities_ = new smart_objects::SmartObject(display_capabilities);
}

void HMICapabilities::set_hmi_zone_capabilities(
    const smart_objects::SmartObject& hmi_zone_capabilities) {
  if (hmi_zone_capabilities_) {
    delete hmi_zone_capabilities_;
  }
  hmi_zone_capabilities_ = new smart_objects::SmartObject(
      hmi_zone_capabilities);
}

void HMICapabilities::set_soft_button_capabilities(
    const smart_objects::SmartObject& soft_button_capabilities) {
  if (soft_buttons_capabilities_) {
    delete soft_buttons_capabilities_;
  }
  soft_buttons_capabilities_ = new smart_objects::SmartObject(
      soft_button_capabilities);
}

void HMICapabilities::set_button_capabilities(
    const smart_objects::SmartObject& button_capabilities) {
  if (button_capabilities_) {
    delete button_capabilities_;
  }
  button_capabilities_ = new smart_objects::SmartObject(button_capabilities);
}

void HMICapabilities::set_vr_capabilities(
    const smart_objects::SmartObject& vr_capabilities) {
  if (vr_capabilities_) {
    delete vr_capabilities_;
  }
  vr_capabilities_ = new smart_objects::SmartObject(vr_capabilities);
}

void HMICapabilities::set_speech_capabilities(
    const smart_objects::SmartObject& speech_capabilities) {
  if (speech_capabilities_) {
    delete speech_capabilities_;
  }
  speech_capabilities_ = new smart_objects::SmartObject(speech_capabilities);
}

void HMICapabilities::set_audio_pass_thru_capabilities(
    const smart_objects::SmartObject& audio_pass_thru_capabilities) {
  if (audio_pass_thru_capabilities_) {
    delete audio_pass_thru_capabilities_;
  }
  audio_pass_thru_capabilities_ = new smart_objects::SmartObject(
      audio_pass_thru_capabilities);
}

void HMICapabilities::set_preset_bank_capabilities(
    const smart_objects::SmartObject& preset_bank_capabilities) {
  if (preset_bank_capabilities_) {
    delete preset_bank_capabilities_;
  }
  preset_bank_capabilities_ = new smart_objects::SmartObject(
      preset_bank_capabilities);
}

}  //  namespace application_manager
