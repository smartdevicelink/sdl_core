/*
 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "sdl_rpc_plugin/sdl_app_extension.h"
#include "sdl_rpc_plugin/sdl_rpc_plugin.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "SDLAppExtension")

namespace sdl_rpc_plugin {
namespace strings = application_manager::strings;
unsigned SDLAppExtension::SDLAppExtensionUID = 138;

SDLAppExtension::SDLAppExtension(SDLRPCPlugin& plugin,
                                 application_manager::Application& app)
    : app_mngr::AppExtension(SDLAppExtension::SDLAppExtensionUID)
    , plugin_(plugin)
    , app_(app) {
  LOG4CXX_AUTO_TRACE(logger_);
}

SDLAppExtension::~SDLAppExtension() {
  LOG4CXX_AUTO_TRACE(logger_);
}

void SDLAppExtension::SaveResumptionData(
    smart_objects::SmartObject& resumption_data) {
  plugin_.SaveResumptionData(app_, resumption_data);
}

void SDLAppExtension::ProcessResumption(
    const smart_objects::SmartObject& saved_app,
    resumption::Subscriber subscriber) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!saved_app.keyExists(strings::subscribed_for_way_points)) {
    LOG4CXX_ERROR(logger_, "subscribed_for_way_points section does not exist");
    return;
  }
  const bool subscribed_for_way_points_so =
      saved_app[strings::subscribed_for_way_points].asBool();
  if (subscribed_for_way_points_so) {
    plugin_.ProcessResumptionSubscription(app_, *this, subscriber);
  }
}

void SDLAppExtension::RevertResumption(
    const smart_objects::SmartObject& subscriptions) {
  plugin_.RevertResumption(app_);
}
}
