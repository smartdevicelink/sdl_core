/**
* \file signals.cc
* \brief Signal (i.e. SIGINT) handling.
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

#include <cstdio>
#include "./life_cycle.h"

namespace main_namespace {
log4cxx::LoggerPtr ApplicationManagerImpl::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("appMain"));

LifeCycle()
  : transport_manager_(NULL)
  , protocol_handler_(NULL)
  , mmh_(NULL)
  , connection_handler_(NULL)
  , app_manager_(NULL)
  , hmi_handler_(NULL)
  , message_broker_(NULL) {
}

LifeCycle* LifeCycle::instance() {
  static LifeCycle instance;
  return &instance;
}

void LifeCycle::StopComponents(int params) {
  printf("\n\n\n\n\t\t\t\t%s\n\n\n\n\n", "BARBARA STRAIZANT");
  delete app_manager_;
  delete connection_handler_;
  delete mmh_;
  delete protocol_handler_;
  delete transport_manager_;
  delete hmi_handler_;
  delete message_broker_;
}
}  //  namespace main_namespace
