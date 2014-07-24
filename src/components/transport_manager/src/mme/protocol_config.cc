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
#include "config_profile/profile.h"

#include "transport_manager/mme/protocol_config.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

const ProtocolConfig::ProtocolNameContainer& ProtocolConfig::LegacyProtocolNames() {
  static ProtocolNameContainer protocol_names = ReadLegacyProtocolNames();
  return protocol_names;
}

const ProtocolConfig::ProtocolNameContainer& ProtocolConfig::HubProtocolNames() {
  static ProtocolNameContainer protocol_names = ReadHubProtocolNames();
  return protocol_names;
}

const ProtocolConfig::ProtocolNameContainer& ProtocolConfig::IAPPoolProtocolNames() {
  static ProtocolNameContainer protocol_names = ReadIAPPoolProtocolNames();
  return protocol_names;
}

const ProtocolConfig::ProtocolNameContainer& ProtocolConfig::IAP2PoolProtocolNames() {
  static ProtocolNameContainer protocol_names = ReadIAP2PoolProtocolNames();
  return protocol_names;
}

const ProtocolConfig::ProtocolNameContainer ProtocolConfig::ReadLegacyProtocolNames() {
  const std::string& legacy_protocol = profile::Profile::instance()->iap_legacy_protocol();
  ProtocolNameContainer protocol_names;
  protocol_names.push_back(legacy_protocol);
  return protocol_names;
}

const ProtocolConfig::ProtocolNameContainer ProtocolConfig::ReadHubProtocolNames() {
  const std::string& hub_protocol = profile::Profile::instance()->iap_hub_protocol();
  ProtocolNameContainer protocol_names;
  protocol_names.push_back(hub_protocol);
  return protocol_names;
}

const ProtocolConfig::ProtocolNameContainer ProtocolConfig::ReadIAPPoolProtocolNames() {
  const std::string& iap_system_config = profile::Profile::instance()->iap_system_config();
  std::string section_name = "[eaf]";
  return ReadProtocolNames(iap_system_config, section_name);
}

const ProtocolConfig::ProtocolNameContainer ProtocolConfig::ReadIAP2PoolProtocolNames() {
  const std::string& iap2_system_config = profile::Profile::instance()->iap2_system_config();
  std::string section_name = "[eap]";
  return ReadProtocolNames(iap2_system_config, section_name);
}

const ProtocolConfig::ProtocolNameContainer ProtocolConfig::ReadProtocolNames(const std::string& config_file_name, const std::string& section_name) {
  ProtocolNameContainer protocol_names;
  std::ifstream config_file(config_file_name);
  if (!config_file.fail()) {
    LOG4CXX_TRACE(logger_, "parsing system config file " << config_file_name << " (section " << section_name << ")");
    const std::string& legacy_protocol = profile::Profile::instance()->iap_legacy_protocol();
    const std::string& hub_protocol = profile::Profile::instance()->iap_hub_protocol();
    std::string line;
    while (std::getline(config_file, line)) {
      if (section_name == line) { // start of specified section
        while (std::getline(config_file, line)) {
          if (line.empty()) { // end of specified section
            break;
          }
          const size_t name_pos = 9; // protocol name start position
          if ("protocol=" == line.substr(0, name_pos)) {
            std::string tail = line.substr(name_pos);
            size_t comma_pos = tail.find_first_of(','); // comma position, can be std::string::npos
            std::string protocol_name = tail.substr(0, comma_pos);
            if (legacy_protocol == protocol_name) {
              LOG4CXX_DEBUG(logger_, "skipping legacy protocol " << protocol_name);
            }
            else if (hub_protocol == protocol_name) {
              LOG4CXX_DEBUG(logger_, "skipping hub protocol " << protocol_name);
            }
            else {
              LOG4CXX_DEBUG(logger_, "adding protocol " << protocol_name);
              protocol_names.push_back(protocol_name);
            }
          }
        }
        break; // nothing matters after specified section
      }
    }
    config_file.close();
    LOG4CXX_TRACE(logger_, "system config file " << config_file_name << " (section " << section_name << ") parsed");
  }
  else {
    LOG4CXX_ERROR(logger_, "cannot open system config file " << config_file_name);
  }
  return protocol_names;
}

}  // namespace transport_adapter
}  // namespace transport_manager
