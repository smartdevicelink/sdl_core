/*
 * Copyright (c) 2017, Ford Motor Company
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

#ifndef SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_SETTINGS_H_
#define SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_SETTINGS_H_

#include <string>
#include <sstream>
#include "json/json.h"
#include "utils/macro.h"

namespace functional_modules {

/*
 * @brief Settings class reads configuration for app from
 * default location - smartDeviceLink.ini - or set location
 * and provides access to them basing on template methods.
 */
class Settings {
 public:
  /*
   * @brief Constrctor
   */
  Settings();

  /*
   * @brief Destructor
   */
  virtual ~Settings();

  /*
   * @brief Template method reads value from config file
   * stored under param_name in section_name paragraph
   * @param section_name Section name i.e [HMI]
   * @param param_name Parameter name i.e. ServerAddress
   * @returns True if value was read from config file, false otherwise
   */
  template <class T>
  bool ReadParameter(const std::string& section_name,
                     const std::string& param_name,
                     T* param_value) const;

  /*
   * @brief Template method reads value from config file
   * stored under param_name in [MAIN] paragraph
   * @param param_name Parameter name i.e. ServerAddress
   * @returns True if value was read from config file, false otherwise
   */
  template <class T>
  bool ReadParameter(const std::string& param_name, T* param_value) const;

  /*
   * @brief Changes path to configuration file
   * @param new_path Path to config file
   */
  void ChangeConfigFile(const std::string& new_path);

 protected:
  virtual std::string ReadParameter(const std::string& section_name,
                                    const std::string& param_name) const;

 private:
  const std::string kConfigFile_;
  std::string config_file_;
};

template <class T>
bool Settings::ReadParameter(const std::string& section_name,
                             const std::string& param_name,
                             T* param_value) const {
  DCHECK(param_value);
  bool result = false;
  if (!param_value) {
    return result;
  }

  std::string param_string = ReadParameter(section_name, param_name);

  if (!param_string.empty()) {
    std::stringstream stream(param_string);
    stream >> (*param_value);
    result = true;
  }

  return result;
}

template <class T>
bool Settings::ReadParameter(const std::string& param_name,
                             T* param_value) const {
  return ReadParameter<T>("MAIN", param_name, param_value);
}

}  // namespace functional_modules

#endif  // SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_SETTINGS_H_
