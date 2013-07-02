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
#include "SmartObjects/CSmartObject.hpp"

namespace application_manager {

HMICapabilities::HMICapabilities()
  : attenuated_supported_(false),
    ui_supported_languages_(NULL),
    tts_supported_languages_(NULL),
    vr_supported_languages_(NULL),
    display_capabilities_(NULL),
    hmi_zone_capabilities_(NULL),
    soft_buttons_capabilities_(NULL),
    button_capabilities_(NULL),
    preset_bank_capabilities_(NULL){}

HMICapabilities::~HMICapabilities() {
  delete ui_supported_languages_;
  delete tts_supported_languages_;
  delete vr_supported_languages_;
  delete display_capabilities_;
  delete hmi_zone_capabilities_;
  delete soft_buttons_capabilities_;
  delete button_capabilities_;
  delete preset_bank_capabilities_;
}

bool HMICapabilities::attenuated_supported() const {
  return attenuated_supported_;
}

void HMICapabilities::set_attenuated_supported(bool state) {
  attenuated_supported_ = state;
}

void HMICapabilities::set_ui_supported_languages(
  const smart_objects::CSmartObject& supported_languages) {
  if (ui_supported_languages_) {
    delete ui_supported_languages_;
  }
  ui_supported_languages_ =
    new smart_objects::CSmartObject(supported_languages);
}

void HMICapabilities::set_tts_supported_languages(
  const smart_objects::CSmartObject& supported_languages) {
  if (tts_supported_languages_) {
    delete tts_supported_languages_;
  }
  tts_supported_languages_ =
    new smart_objects::CSmartObject(supported_languages);
}

void HMICapabilities::set_vr_supported_languages(
  const smart_objects::CSmartObject& supported_languages) {
  if (vr_supported_languages_) {
    delete vr_supported_languages_;
  }
  vr_supported_languages_ =
    new smart_objects::CSmartObject(supported_languages);
}

void HMICapabilities::set_display_capabilities(
  const smart_objects::CSmartObject& display_capabilities) {
  if (display_capabilities_) {
    delete display_capabilities_;
  }
  display_capabilities_ =
    new smart_objects::CSmartObject(display_capabilities);
}

void HMICapabilities::set_hmi_zone_capabilities(
  const smart_objects::CSmartObject& hmi_zone_capabilities) {
  if (hmi_zone_capabilities_) {
    delete hmi_zone_capabilities_;
  }
  hmi_zone_capabilities_ =
    new smart_objects::CSmartObject(hmi_zone_capabilities);
}

void HMICapabilities::set_soft_button_capabilities(
  const smart_objects::CSmartObject& soft_button_capabilities) {
  if (soft_buttons_capabilities_) {
    delete soft_buttons_capabilities_;
  }
  soft_buttons_capabilities_ =
    new smart_objects::CSmartObject(soft_button_capabilities);
}

void HMICapabilities::set_button_capabilities(
    const smart_objects::CSmartObject& button_capabilities) {
  if (button_capabilities_) {
     delete button_capabilities_;
   }
  button_capabilities_ =
     new smart_objects::CSmartObject(button_capabilities);
}

void HMICapabilities::set_preset_bank_capabilities(
    const smart_objects::CSmartObject& preset_bank_capabilities) {
  if (preset_bank_capabilities_) {
     delete preset_bank_capabilities_;
   }
  preset_bank_capabilities_ =
     new smart_objects::CSmartObject(preset_bank_capabilities);
}


}  //  namespace application_manager
