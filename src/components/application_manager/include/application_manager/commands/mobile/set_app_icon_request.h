/*

 Copyright (c) 2013, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_SET_ICON_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_SET_ICON_REQUEST_H_

#include "application_manager/commands/command_request_impl.h"
#include "utils/macro.h"

namespace application_manager {

namespace commands {

/**
 * @brief SetIconRequest command class
 **/
class SetAppIconRequest : public CommandRequestImpl {
 public:
  /**
   * @brief Contains information about the type of image
   */
  typedef enum {
    STATIC = 0,
    DYNAMIC
  } ImageType;

  /**
   * @brief SetIconRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  explicit SetAppIconRequest(const MessageSharedPtr& message);

  /**
   * @brief SetIconRequest class destructor
   **/
  virtual ~SetAppIconRequest();

  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  void on_event(const event_engine::Event& event);

  /**
   * @brief Execute command
   **/
  virtual void Run();

 private:
  /**
   * @brief Copies file to icon storage
   * @param path_to_file Path to icon
   */
  void CopyToIconStorage(const std::string& path_to_file) const;

  /**
   * @brief Remove oldest icons
   * @param storage Path to icons storage
   * @param icons_amount Amount of icons to be deleted
   */
  void RemoveOldestIcons(const std::string& storage,
                         const uint32_t icons_amount) const;

  /**
   * @brief Checks, if there enough space in storage for icon copy
   * @param file_size File size
   * @return true, if enough, otherwise - false
   */
  bool IsEnoughSpaceForIcon(const uint64_t icon_size) const;
  DISALLOW_COPY_AND_ASSIGN(SetAppIconRequest);

private:
  /**
   * @brief Checks, if icons saving to configured folder is enabled
   */
  static int8_t is_icons_saving_enabled_;
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_SET_ICON_REQUEST_H_
