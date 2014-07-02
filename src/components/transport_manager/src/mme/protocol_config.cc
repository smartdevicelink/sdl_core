/*
 * Copyright (c) 2014, Ford Motor Company
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

#include <fstream>

#include "utils/logger.h"

#include "transport_manager/mme/protocol_config.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

const char* ProtocolConfig::config_file_name_ = "prot.cfg";

const ProtocolConfig::ProtocolNameContainer& ProtocolConfig::LegacyProtocolNames() {
  static ProtocolNameContainer protocol_names = ReadLegacyProtocolNames();
  return protocol_names;
}

const ProtocolConfig::ProtocolNameContainer& ProtocolConfig::HubProtocolNames() {
  static ProtocolNameContainer protocol_names = ReadHubProtocolNames();
  return protocol_names;
}

const ProtocolConfig::ProtocolNameContainer& ProtocolConfig::PoolProtocolNames() {
  static ProtocolNameContainer protocol_names = ReadPoolProtocolNames();
  return protocol_names;
}

const ProtocolConfig::ProtocolNameContainer ProtocolConfig::ReadLegacyProtocolNames() {
  return ReadProtocolNames("[legacy]");
}

const ProtocolConfig::ProtocolNameContainer ProtocolConfig::ReadHubProtocolNames() {
  return ReadProtocolNames("[hub]");
}

const ProtocolConfig::ProtocolNameContainer ProtocolConfig::ReadPoolProtocolNames() {
  return ReadProtocolNames("[pool]");
}

const ProtocolConfig::ProtocolNameContainer ProtocolConfig::ReadProtocolNames(const std::string& section_name) {
  ProtocolNameContainer protocol_names;

  LOG4CXX_TRACE(logger_, "parsing protocol config file " << config_file_name_ << " (section " << section_name << ")");
  std::ifstream config_file(config_file_name_);
  std::string line;
  while (std::getline(config_file, line)) {
    if (section_name == line) { // start of specified section
      while (std::getline(config_file, line)) {
        if (line.empty()) { // end of specified section
          break;
        }
        LOG4CXX_DEBUG(logger_, "adding protocol " << line);
        protocol_names.push_back(line);
      }
      break; // nothing matters after specified section
    }
  }
  config_file.close();
  LOG4CXX_TRACE(logger_, "protocol config file " << config_file_name_ << " (section " << section_name << ") parsed");
  return protocol_names;
}

}  // namespace transport_adapter
}  // namespace transport_manager
