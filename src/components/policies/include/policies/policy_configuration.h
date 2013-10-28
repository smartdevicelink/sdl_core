/**
 * @file policy_configuration.h
 * @brief Policy configuration header file.
 */
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_POLICY_CONFIGURATION_H_
#define SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_POLICY_CONFIGURATION_H_

#include <string>
#include "smart_objects/smart_object.h"
#include "smart_objects/smart_schema.h"

namespace NsSmartDeviceLink {
namespace policies {

/**
 * @brief Class contains configuration for policy table
 */
class PolicyConfiguration {
  public:
    /**
    * @brief Constructor
    */
    PolicyConfiguration();

    /**
    * @brief set file path (with name) for policy table
    *
    * @param pt_file_path File path
    */
    void setPTFileName(std::string pt_file_path);
    
    /**
     * @brief set file path (with name) for preload policy table
     *
     * @param preload_pt_file_path File path
     **/
    void setPreloadPTFileName(std::string preload_pt_file_path);

    /**
     * @brief Get file path of policy table
     *
     * @return File path
     */
    std::string getPTFileName() const;
    
    /**
     * @brief Get file path of preload policy table
     *
     * @return File path
     **/
    std::string getPreloadPTFileName() const;

  private:
    /**
     * @brief policy table file name
     */
    std::string pt_fname_;
    
    /**
     * @brief Preload policy table file name
     **/
    std::string preload_pt_fname_;
};

}  // namespace policies
}  // namespace NsSmartDeviceLink


#endif  // SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_POLICY_CONFIGURATION_H_
