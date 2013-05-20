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

#ifndef SRC_COMPONENTS_CONFIG_PROFILE_INCLUDE_CONFIG_PROFILE_PROFILE_H_
#define SRC_COMPONENTS_CONFIG_PROFILE_INCLUDE_CONFIG_PROFILE_PROFILE_H_

#include <string>
#include "utils/macro.h"

namespace profile {

/**
 * The Profile class
 */
class Profile {
  public:
  // Methods section

  /**
   * @brief Returns the singleton of class
   */
  static Profile* getInstance();

  /**
   * Destructor
   *
   * Unimplemented to avoid misusing
   *
   */
  virtual ~Profile();

  /**
   * @brief Returns the path to the config file
   */
  inline const std::string& getConfigFileName() const;

  /**
   * @brief Sets the path to the config file
   */
  void setConfigFileName(const std::string& fileName);

  /**
   * @brief Returns server address
   */
  const std::string& getServerAddress() const;

  /**
   * @brief Returns server port
   */
  const uint16_t& getServerPort() const;

  /**
   * @brief Returns desirable thread stack size
   */
  const uint64_t& getThreadStachSize() const;

  // Members section

  protected:
  // Methods section

  // Members section

  private:
  // Methods section

  /**
   * Default constructor
   *
   * Unimplemented to avoid misusing
   *
   */
  Profile();

  /*
   * @brief Updates all related values from ini file
   */
  void updateValues();

  /**
   * @brief Reads a boolean value from the profile
   *
   * @param value      The value to return
   * @param pSection   The section to read the value in
   * @param pKey       The key whose value needs to be read out
   *
   * @return FALSE if could not read the value out of the profile (then the value is not changed)
   */
  bool readValue(bool* value,
                 const char* const pSection,
                 const char* const pKey) const;

  /**
   * @brief Reads a string value from the profile
   *
   * @param value      The value to return
   * @param pSection   The section to read the value in
   * @param pKey       The key whose value needs to be read out
   *
   * @return FALSE if could not read the value out of the profile (then the value is not changed)
   */
  bool readValue(std::string* value,
                 const char* const pSection,
                 const char* const pKey) const;

  // Members section
  std::string  configFileName_;
  std::string  serverAddress_;
  uint16_t     serverport_;
  uint64_t     minTreadStackSize_;

  DISALLOW_COPY_AND_ASSIGN(Profile);
};

}  //  namespace profile

#endif  // SRC_COMPONENTS_CONFIG_PROFILE_INCLUDE_CONFIG_PROFILE_PROFILE_H_
