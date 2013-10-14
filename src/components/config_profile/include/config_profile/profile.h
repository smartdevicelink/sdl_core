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
#include <vector>
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
    static Profile* instance();

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
    const std::string& config_file_name() const;

    /**
     * @brief Sets the path to the config file
     */
    void config_file_name(const std::string& fileName);

    /**
     * @brief Returns server address
     */
    const std::string& server_address() const;

    /**
     * @brief Returns policies file name
     */
    const std::string& policies_file_name() const;

    /**
     * @brief Returns help promt vector
     */
    const std::vector<std::string>& help_promt() const;

    /**
     * @brief Returns help promt vector
     */
    const std::vector<std::string>& time_out_promt() const;

    /**
     * @brief Returns vr commands default for all apps
     * such as Help.
     */
    const std::vector<std::string>& vr_commands() const;

    /**
     * @brief Maximum command id available for mobile app
     */
    const unsigned int max_cmd_id() const;

    /**
     * @brief Default timeout for waiting for response to mobile app
     */
    const unsigned int default_timeout() const;

    /**
     * @brief Returns server port
     */
    const uint16_t& server_port() const;

    /**
     * @brief Returns desirable thread stack size
     */
    const uint64_t& thread_min_stach_size() const;

    /**
      * @brief Returns true if audio mixing is supported
      */
    bool is_mixing_audio_supported() const;

    /**
      * @brief Returns true if video re-decoding enabled
      */
    bool is_redecoding_enabled() const;

    /**
      * @brief Returns title for Vr Help
      */
    const std::string& vr_help_title() const;

    /**
      * @brief Returns space for app
      */
    const unsigned int space_available() const;

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
    void UpdateValues();

    /**
     * @brief Reads a boolean value from the profile
     *
     * @param value      The value to return
     * @param pSection   The section to read the value in
     * @param pKey       The key whose value needs to be read out
     *
     * @return FALSE if could not read the value out of the profile (then the value is not changed)
     */
    bool ReadValue(bool* value,
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
    bool ReadValue(std::string* value,
                   const char* const pSection,
                   const char* const pKey) const;

    // Members section
    std::string                     config_file_name_;
    std::string                     server_address_;
    std::string                     policies_file_name_;
    std::vector<std::string>        help_promt_;
    std::vector<std::string>        time_out_promt_;
    std::vector<std::string>        vr_commands_;
    uint16_t                        server_port_;
    uint64_t                        min_tread_stack_size_;
    bool                            is_mixing_audio_supported_;
    bool                            is_redecoding_enabled_;
    unsigned int                    max_cmd_id_;
    unsigned int                    default_timeout_;
    std::string                     vr_help_title_;
    unsigned int                    space_available_;

    DISALLOW_COPY_AND_ASSIGN(Profile);
};

}  //  namespace profile

#endif  // SRC_COMPONENTS_CONFIG_PROFILE_INCLUDE_CONFIG_PROFILE_PROFILE_H_
