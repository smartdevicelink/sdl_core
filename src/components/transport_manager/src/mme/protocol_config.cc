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

#include <regex.h>

#include <fstream>

#include "utils/logger.h"
#include "config_profile/profile.h"

#include "transport_manager/mme/protocol_config.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

// shall we move it to config?
const std::string ProtocolConfig::iap_section_name = "[eaf]";
const std::string ProtocolConfig::iap2_section_name = "[eap]";

const ProtocolConfig::ProtocolNameContainer& ProtocolConfig::IAPLegacyProtocolNames() {
  static ProtocolNameContainer protocol_names = ReadIAPLegacyProtocolNames();
  return protocol_names;
}

const ProtocolConfig::ProtocolNameContainer& ProtocolConfig::IAP2LegacyProtocolNames() {
  static ProtocolNameContainer protocol_names = ReadIAP2LegacyProtocolNames();
  return protocol_names;
}

const ProtocolConfig::ProtocolNameContainer& ProtocolConfig::IAPHubProtocolNames() {
  static ProtocolNameContainer protocol_names = ReadIAPHubProtocolNames();
  return protocol_names;
}

const ProtocolConfig::ProtocolNameContainer& ProtocolConfig::IAP2HubProtocolNames() {
  static ProtocolNameContainer protocol_names = ReadIAP2HubProtocolNames();
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

const ProtocolConfig::ProtocolNameContainer ProtocolConfig::ReadIAPLegacyProtocolNames() {
  const std::string& iap_system_config = profile::Profile::instance()->iap_system_config();
  const std::string& legacy_protocol_mask = profile::Profile::instance()->iap_legacy_protocol_mask();
  return ReadProtocolNames(iap_system_config, iap_section_name, legacy_protocol_mask);
}

const ProtocolConfig::ProtocolNameContainer ProtocolConfig::ReadIAP2LegacyProtocolNames() {
  const std::string& iap2_system_config = profile::Profile::instance()->iap2_system_config();
  const std::string& legacy_protocol_mask = profile::Profile::instance()->iap_legacy_protocol_mask();
  return ReadProtocolNames(iap2_system_config, iap2_section_name, legacy_protocol_mask);
}

const ProtocolConfig::ProtocolNameContainer ProtocolConfig::ReadIAPHubProtocolNames() {
  const std::string& iap_system_config = profile::Profile::instance()->iap_system_config();
  const std::string& hub_protocol_mask = profile::Profile::instance()->iap_hub_protocol_mask();
  return ReadProtocolNames(iap_system_config, iap_section_name, hub_protocol_mask);
}

const ProtocolConfig::ProtocolNameContainer ProtocolConfig::ReadIAP2HubProtocolNames() {
  const std::string& iap2_system_config = profile::Profile::instance()->iap2_system_config();
  const std::string& hub_protocol_mask = profile::Profile::instance()->iap_hub_protocol_mask();
  return ReadProtocolNames(iap2_system_config, iap2_section_name, hub_protocol_mask);
}

const ProtocolConfig::ProtocolNameContainer ProtocolConfig::ReadIAPPoolProtocolNames() {
  const std::string& iap_system_config = profile::Profile::instance()->iap_system_config();
  const std::string& pool_protocol_mask = profile::Profile::instance()->iap_pool_protocol_mask();
  return ReadProtocolNames(iap_system_config, iap_section_name, pool_protocol_mask);
}

const ProtocolConfig::ProtocolNameContainer ProtocolConfig::ReadIAP2PoolProtocolNames() {
  const std::string& iap2_system_config = profile::Profile::instance()->iap2_system_config();
  const std::string& pool_protocol_mask = profile::Profile::instance()->iap_pool_protocol_mask();
  return ReadProtocolNames(iap2_system_config, iap2_section_name, pool_protocol_mask);
}

const ProtocolConfig::ProtocolNameContainer ProtocolConfig::ReadProtocolNames(const std::string& config_file_name,
                                                                              const std::string& section_name,
                                                                              const std::string& protocol_mask) {
  ProtocolNameContainer protocol_names;
  std::ifstream config_file(config_file_name.c_str());
  if (!config_file.fail()) {
    LOG4CXX_TRACE(logger_, "parsing system config file " << config_file_name
                            << " (section " << section_name << ", protocol mask \"" << protocol_mask << "\")");
    std::string line;
    while (std::getline(config_file, line)) {
      if (section_name == line) { // start of specified section
        while (std::getline(config_file, line)) {
          if (line.empty()) { // end of specified section
            break;
          }
          int prot_mask_pos = line.find(protocol_mask);
          if (prot_mask_pos == std::string::npos){  // protocol_mask not found in line
              continue;
          }
          std::string tail = line.substr(prot_mask_pos + protocol_mask.length());
          size_t comma_pos = tail.find_first_of(','); // comma position, can be std::string::npos
          std::string digits_in_prot_name = tail.substr(0, comma_pos);
          std::string prot_name = protocol_mask + digits_in_prot_name;

          int index;
          std::stringstream stream(digits_in_prot_name);
          stream >> index;

          LOG4CXX_DEBUG(logger_, "adding protocol " << prot_name);
          protocol_names.insert(make_pair(index, prot_name));
        }
        break; // nothing matters after specified section
      }
    }
    config_file.close();
    LOG4CXX_TRACE(logger_, "system config file " << config_file_name << " (section " << section_name << ", protocol mask \"" << protocol_mask << "\") parsed");
  }
  else {
    LOG4CXX_ERROR(logger_, "cannot open system config file " << config_file_name);
  }
  return protocol_names;
}

}  // namespace transport_adapter
}  // namespace transport_manager
