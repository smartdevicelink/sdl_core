/*
 * Copyright (c) 2016, Ford Motor Company
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

#include <stdint.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "utils/macro.h"
#include "protocol_handler/protocol_handler_settings.h"
#include "connection_handler/connection_handler_settings.h"
#include "hmi_message_handler/hmi_message_handler_settings.h"
#include "media_manager/media_manager_settings.h"
#include "transport_manager/transport_manager_settings.h"
#include "application_manager/application_manager_settings.h"
#include "policy/policy_settings.h"

namespace profile {

/**
 * The Profile class
 */
class Profile : public protocol_handler::ProtocolHandlerSettings,
                public connection_handler::ConnectionHandlerSettings,
                public hmi_message_handler::HMIMessageHandlerSettings,
                public media_manager::MediaManagerSettings,
                public policy::PolicySettings,
                public transport_manager::TransportManagerSettings,
                public application_manager::ApplicationManagerSettings {
 public:
  // Methods section

  /**
   * Default constructor
   *
   * Unimplemented to avoid misusing
   *
   */
  Profile();

  /**
   * Destructor
   *
   * Unimplemented to avoid misusing
   *
   */
  virtual ~Profile();

  /**
   * @brief Returns sdl version represented
   * by git commit or value specified by user
   */
  const std::string& sdl_version() const OVERRIDE;

  /**
    * @brief Returns true if HMI should be started, otherwise false
    */
  bool launch_hmi() const OVERRIDE;
#ifdef WEB_HMI
  /**
    * @brief Returns link to web hmi
    */
  std::string link_to_web_hmi() const;
#endif  // WEB_HMI
        /**
          * @brief Returns application configuration path
          */
  const std::string& app_config_folder() const;

  /**
    * @brief Returns application storage path
    */
  const std::string& app_storage_folder() const;

  /**
   * @brief Return application resourse folder
   */
  const std::string& app_resource_folder() const;

  /**
   * @brief Returns true, if SDL 4.0 is enabled
   * @deprecated use max_supported_protocol_version instead
   */
  DEPRECATED bool enable_protocol_4() const OVERRIDE;

  /**
   * @brief Returns application icons folder path
   */
  const std::string& app_icons_folder() const OVERRIDE;

  /**
   * @brief Returns application icons folder maximum size
   */
  const uint32_t& app_icons_folder_max_size() const OVERRIDE;

  /**
   * @brief Returns application icons amount to remove from icon folder,
   * if maximum size exceeded
   */
  const uint32_t& app_icons_amount_to_remove() const OVERRIDE;

  /**
   * @brief Returns the maximum payload size for control services
   */
  size_t maximum_control_payload_size() const OVERRIDE;

  /**
   * @brief Returns the maximum payload size for RPC services
   */
  size_t maximum_rpc_payload_size() const OVERRIDE;

  /**
   * @brief Returns the maximum payload size for audio services
   */
  size_t maximum_audio_payload_size() const OVERRIDE;

  /**
   * @brief Returns the maximum payload size for video services
   */
  size_t maximum_video_payload_size() const OVERRIDE;

  /**
   * @brief Returns the path to the config file
   */
  const std::string& config_file_name() const;

  /**
   * @brief Sets the path to the config file
   */
  void set_config_file_name(const std::string& file_name);

  /**
   * @brief Returns server address
   */
  const std::string& server_address() const;

  /**
   * @brief Returns server port
   */
  const uint16_t& server_port() const;

  /**
   * @brief Returns port for video streaming
   */
  const uint16_t video_streaming_port() const OVERRIDE;

  /**
   * @brief Returns port for audio streaming
   */
  const uint16_t audio_streaming_port() const OVERRIDE;

  /**
   * @brief Returns streaming timeout
   */
  uint32_t stop_streaming_timeout() const;

  /**
    * @brief Returns port for time reports
    */
  const uint16_t& time_testing_port() const;

  /**
   * @brief Returns hmi capabilities file name
   */
  const std::string& hmi_capabilities_file_name() const;

  /**
   * @brief Returns help promt vector
   */
  const std::vector<std::string>& help_prompt() const;

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
    * @brief Returns folder containing all plugins
    */
  const std::string& plugins_folder() const OVERRIDE;

  /**
   * @brief Maximum command id available for mobile app
   */
  const uint32_t& max_cmd_id() const;

  /**
   * @brief Default timeout for waiting for response to mobile app
   */
  const uint32_t& default_timeout() const;

  /**
   * @brief Default timeout for waiting for resuming
   */
  const uint32_t& app_resuming_timeout() const;
  const uint32_t& app_resumption_save_persistent_data_timeout() const;

  /**
   * @brief Returns desirable thread stack size
   */
  const uint64_t thread_min_stack_size() const;

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
    * @brief Returns application directory quota size
    */
  const uint32_t& app_dir_quota() const;

  /**
    * @brief Returns the video server type
    */
  const std::string& video_server_type() const;

  /**
    * @brief Returns the audio server type
    */
  const std::string& audio_server_type() const;

  /**
    * @brief Returns the video pipe path
    */
  const std::string& named_video_pipe_path() const;

  /**
   * @brief Returns the audio pipe path
   */
  const std::string& named_audio_pipe_path() const;

  /**
   * @brief Returns time scale for max amount of requests for application
   * in hmi level none.
   */
  const uint32_t& app_hmi_level_none_time_scale() const;

  /**
    * @brief Returns path to testing file to which redirects video stream
    */
  const std::string& video_stream_file() const;

  /**
    * @brief Returns path to testing file to which redirects audio stream
    */
  const std::string& audio_stream_file() const;

  /**
   * @brief Returns timeout for SDL to wait for the next package of raw data
   * over audio service
   */
  const uint32_t audio_data_stopped_timeout() const;

  /**
   * @brief Returns timeout for SDL to wait for the next package of raw data
   * over video service
   */
  const uint32_t video_data_stopped_timeout() const;

  /**
   * @brief Returns allowable max amount of requests per time scale for
   * application in hmi level none
   *
   */
  const uint32_t& app_hmi_level_none_time_scale_max_requests() const;

  /**
   * @brief Returns application time scale for max amount of requests per it.
   */
  const uint32_t& app_time_scale() const;

  /**
   * @brief Returns allowable max amount of requests per application
   * time scale
   */
  const uint32_t& app_time_scale_max_requests() const;

  /**
   * @brief Returns allowable amount of the system pending requests
   */
  const uint32_t& pending_requests_amount() const;

  /**
   * @brief Returns Max allowed number of PutFile requests for one
   * application in NONE
   */
  const uint32_t& put_file_in_none() const OVERRIDE;

  /**
   * @brief Returns Max allowed number of DeleteFile requests for one
   * application in NONE
   */
  const uint32_t& delete_file_in_none() const OVERRIDE;

  /**
   * @brief Returns Max allowed number of ListFiles requests for one
   * application in NONE
   */
  const uint32_t& list_files_in_none() const OVERRIDE;

  /*
   * @brief Returns file name for storing applications data
   */
  const std::string& app_info_storage() const OVERRIDE;

  /*
   * @brief Path to preloaded policy file
   */
  const std::string& preloaded_pt_file() const;

  /**
   * @brief Path to policies snapshot file
   * @return file path
   */
  const std::string& policies_snapshot_file_name() const;

  /**
   * @brief Should Policy be turned off? (Library not loaded)
   * @return Flag
   */
  bool enable_policy() const;

  // TransportManageSettings interface

  /*
   * @brief Returns true if last state singleton is used
   */
  bool use_last_state() const OVERRIDE;

  /**
   * @brief Timeout in transport manager before disconnect
   */
  uint32_t transport_manager_disconnect_timeout() const OVERRIDE;

  /**
   * @brief Returns port for TCP transport adapter
   */
  uint16_t transport_manager_tcp_adapter_port() const OVERRIDE;

  /**
   * @brief Returns the network interface name for TCP transport adapter
   */
  const std::string& transport_manager_tcp_adapter_network_interface()
      const OVERRIDE;

  // TransportManageMMESettings interface

  const std::string& event_mq_name() const OVERRIDE;

  const std::string& ack_mq_name() const OVERRIDE;

  uint32_t iap2_hub_connect_attempts() const OVERRIDE;

  uint32_t default_hub_protocol_index() const OVERRIDE;

  const std::string& iap_legacy_protocol_mask() const OVERRIDE;

  const std::string& iap_hub_protocol_mask() const OVERRIDE;

  const std::string& iap_pool_protocol_mask() const OVERRIDE;

  const std::string& iap_system_config() const OVERRIDE;

  const std::string& iap2_system_config() const OVERRIDE;

  uint32_t iap_hub_connection_wait_timeout() const OVERRIDE;
  // TransportManageSettings interface end

  /**
   * @brief Returns supported diagnostic modes
   */
  const std::vector<uint32_t>& supported_diag_modes() const OVERRIDE;

  /**
    * @brief Returns system files folder path
    */
  const std::string& system_files_path() const OVERRIDE;

  /**
   * @brief Returns value of timeout after which sent
   * tts global properties for VCA
   */
  uint16_t tts_global_properties_timeout() const OVERRIDE;

#ifdef ENABLE_SECURITY
  /**
   * @brief Returns name of Security Manager protocol
   */
  const std::string& security_manager_protocol_name() const;

  /**
   * @brief Returns SSL mode
   */
  const std::string& ssl_mode() const;

  /**
   * @brief Returns key path to pem file
   */
  const std::string& key_path() const;

  /**
   * @brief Returns certificate path to pem file
   */
  const std::string& cert_path() const;

  /**
   * @brief Returns ca certificate path to pem file
   */
  const std::string& ca_cert_path() const;

  /**
   * @brief Returns ciphers
   */
  const std::string& ciphers_list() const;

  /**
   * @brief Returns true if Mobile app certificate is verified
   */
  bool verify_peer() const;

  /**
   * @brief Return hours amount when PTU should be triggered
   */
  size_t update_before_hours() const;

#endif  // ENABLE_SECURITY

  /**
   * @brief Returns true multiple transports is enabled
   */
  const bool multiple_transports_enabled() const;

  /**
   * @brief Returns list of secondary transports available
   * for the named primary transport
   */
  const std::vector<std::string>& secondary_transports_for_bluetooth() const;
  const std::vector<std::string>& secondary_transports_for_usb() const;
  const std::vector<std::string>& secondary_transports_for_wifi() const;

  /**
   * @brief Returns list of allowed transports for the named service
   */
  const std::vector<std::string>& audio_service_transports() const;
  const std::vector<std::string>& video_service_transports() const;

  /**
   * @brief Reads a string value from the profile
   *
   * @param value         Result value
   * @param default_value Value to use key wasn't found
   * @param pSection      The section to read the value in
   * @param pKey          The key whose value needs to be read out
   *
   * @return FALSE if could not read the value out of the profile
   * (then the value is equal \c default_value)
   */
  bool ReadStringValue(std::string* value,
                       const char* default_value,
                       const char* const pSection,
                       const char* const pKey) const;

  /**
   * @brief Reads an int32_t value from the profile
   *
   * @param value         Result value
   * @param default_value Value to use key wasn't found
   * @param pSection      The section to read the value in
   * @param pKey          The key whose value needs to be read out
   *
   * @return FALSE if could not read the value out of the profile
   * (then the value is equal \c default_value)
   */
  bool ReadIntValue(int32_t* value,
                    int32_t default_value,
                    const char* const pSection,
                    const char* const pKey) const;
  /**
   * @brief Reads an bool value from the profile
   *
   * @param value         Result value
   * @param default_value Value to use key wasn't found
   * @param pSection      The section to read the value in
   * @param pKey          The key whose value needs to be read out
   *
   * @return FALSE if could not read the value out of the profile
   * (then the value is equal \c default_value)
   */
  bool ReadBoolValue(bool* value,
                     const bool default_value,
                     const char* const pSection,
                     const char* const pKey) const;
  /**
   * @brief Reads an container of string values from the profile,
   * which handle as "Value1, Value2, Value3"
   *
   * @param pSection      The section to read the value in
   * @param pKey          The key whose value needs to be read out
   * @param out_result    Pointer to bool value for result reading Section
   * (could be NULL)
   * @param allow_empty   If true, then out_result will be true when the value
   *                      contains an empty string.
   *                      If false, then out_result will be false in such case.
   *
   * @return container of values or empty continer
   * if could not read the value out of the profile
   */
  std::vector<std::string> ReadStringContainer(const char* const pSection,
                                               const char* const pKey,
                                               bool* out_result,
                                               bool allow_empty = false) const;
  /**
   * @brief Reads an container of hex int values from the profile,
   * which handle as "0x01, 0xA0, 0XFF"
   *
   * @param pSection      The section to read the value in
   * @param pKey          The key whose value needs to be read out
   * @param out_result    Pointer to bool value for result reading Section
   * (could be NULL)
   *
   * @return container of values or empty continer
   * if could not read the value out of the profile
   */
  std::vector<int> ReadIntContainer(const char* const pSection,
                                    const char* const pKey,
                                    bool* out_result) const;

  /**
   * @brief Returns delimiter for SDL-generated TTS chunks
   * @return TTS delimiter
   */
  const std::string& tts_delimiter() const OVERRIDE;

  /**
   * @brief Returns recording file name
   */
  const std::string& recording_file_name() const OVERRIDE;

  uint32_t application_list_update_timeout() const OVERRIDE;

  /**
   * @brief Returns max allowed threads number for handling mobile requests
   */
  uint32_t thread_pool_size() const;

  /*
   * ProtocolHandler section
   */
  size_t maximum_payload_size() const OVERRIDE;

  size_t message_frequency_count() const OVERRIDE;

  size_t message_frequency_time() const OVERRIDE;

  bool malformed_message_filtering() const OVERRIDE;

  size_t malformed_frequency_count() const OVERRIDE;

  size_t malformed_frequency_time() const OVERRIDE;

  uint32_t multiframe_waiting_timeout() const OVERRIDE;

  uint32_t heart_beat_timeout() const OVERRIDE;

  uint16_t max_supported_protocol_version() const OVERRIDE;

#ifdef ENABLE_SECURITY
  const std::vector<int>& force_protected_service() const OVERRIDE;

  const std::vector<int>& force_unprotected_service() const OVERRIDE;
#endif  // ENABLE_SECURITY
  // ProtocolHandler section end

  uint16_t attempts_to_open_policy_db() const;

  uint16_t open_attempt_timeout_ms() const;

  uint32_t resumption_delay_before_ign() const;

  const uint32_t resumption_delay_after_ign() const;

  uint32_t hash_string_size() const;

  bool logs_enabled() const;

  /**
   * @brief Returns true if resumption ctrl uses db, returns false if
   * resumption ctrl uses JSON.
   */
  bool use_db_for_resumption() const;

  /**
   * @brief Returns amount of attempts for opening resumption db
   */
  uint16_t attempts_to_open_resumption_db() const;

  /**
   * @brief Returns timeout between attempts
   */
  uint16_t open_attempt_timeout_ms_resumption_db() const;

  /**
   * @brief Returns "transport required for resumption" map
   *
   * Keys of the map are AppHMIType strings, i.e. "DEFAULT", "COMMUNICATION",
   * "MEDIA", and so on. The map may contain a special key "EMPTY_APP" for apps
   * that does not specify any AppHMIType.
   */
  const std::map<std::string, std::vector<std::string> >&
  transport_required_for_resumption_map() const OVERRIDE;

  /**
   * @brief Returns HMI level for resumption of a NAVIGATION app
   */
  const std::string& navigation_lowbandwidth_resumption_level() const OVERRIDE;

  /**
   * @brief Returns HMI level for resumption of a PROJECTION app
   */
  const std::string& projection_lowbandwidth_resumption_level() const OVERRIDE;

  /**
   * @brief Returns HMI level for resumption of a media app
   *
   * Note: this is not for AppHMIType = MEDIA.
   */
  const std::string& mediaapp_lowbandwidth_resumption_level() const OVERRIDE;

  /**
   * @brief Returns wait time after device connection
   * before app launch request
   */
  const uint16_t app_launch_wait_time() const OVERRIDE;

  /**
   * @brief Returns max number of attempts to launch an application
   * after device connection
   */
  const uint16_t app_launch_max_retry_attempt() const OVERRIDE;

  /**
   * @brief Returns wait time before next app launch request
   */
  const uint16_t app_launch_retry_wait_time() const OVERRIDE;

  /**
   * @brief Returns max number of failed iOS app
   * registration requests
   */
  const uint16_t remove_bundle_id_attempts() const OVERRIDE;

  /**
   * @brief Returns max number of iOS devices to be stored
   */
  const uint16_t max_number_of_ios_device() const OVERRIDE;

  /**
   * @brief Returns wait time before attempt to launch next app
   */
  const uint16_t wait_time_between_apps() const OVERRIDE;

  /**
   * @brief Returns status of feature of app launch on iOS
   */
  const bool enable_app_launch_ios() const OVERRIDE;

  /**
     * @brief Returns the millisecond count before timeout
     * for transport change feature occures.
     */
  uint32_t app_transport_change_timer() const OVERRIDE;

  /**
   * @brief Returns the millisecond count used as addition
   * value for transport change timer
   */
  uint32_t app_transport_change_timer_addition() const OVERRIDE;

  /**
   * @brief Updates all related values from ini file
   */
  void UpdateValues();

  /**
   * @brief Gets reading result of all related values from ini file
   * @returns TRUE if no error appeared during updating
   * otherwise FALSE
   */
  const bool ErrorOccured() const;

  const uint32_t& list_files_response_size() const OVERRIDE;

  const std::string& recording_file_source() const OVERRIDE;

  const std::pair<uint32_t, int32_t>& read_did_frequency() const OVERRIDE;

  const std::pair<uint32_t, int32_t>& get_vehicle_data_frequency()
      const OVERRIDE;

  const std::pair<uint32_t, int32_t>& start_stream_retry_amount()
      const OVERRIDE;

  /**
   * @brief Returns error description
   * @return Actual error description if error appears otherwise empty line
   */
  const std::string ErrorDescription() const;

 private:
  /**
   * @brief Checks that filename consists of portable symbols
   * @param file_name - file name to check
   * @return FALSE if file name has unportable symbols otherwise TRUE
   */
  bool IsFileNamePortable(const std::string& file_name) const;

  /**
   * @brief Reads a string value from the profile and interpret it
   * as \c true on "true" value or as \c false on any other value
   *
   * @param value      The value to return
   * @param pSection   The section to read the value in
   * @param pKey       The key whose value needs to be read out
   *
   * @return FALSE if could not read the value out of the profile
   * (then the value is not changed) or the value was empty
   */
  bool ReadValue(std::string* value,
                 const char* const pSection,
                 const char* const pKey) const;

  /**
   * @brief Reads a string value from the profile
   *
   * This is same as ReadValue(), except that this method will accept an empty
   * string.
   *
   * @param value      The value to return
   * @param pSection   The section to read the value in
   * @param pKey       The key whose value needs to be read out
   *
   * @return TRUE if the value is read, FALSE if the value is not found
   */
  bool ReadValueEmpty(std::string* value,
                      const char* const pSection,
                      const char* const pKey) const;

  /**
   * @brief Reads a boolean value from the profile
   *
   * @param value      The value to return
   * @param pSection   The section to read the value in
   * @param pKey       The key whose value needs to be read out
   *
   * @return FALSE if could not read the value out of the profile
   * (then the value is not changed)
   */
  bool ReadValue(bool* value,
                 const char* const pSection,
                 const char* const pKey) const;

  /**
   * @brief Reads a pair of ints value from the profile
   *
   * @param value         Result value
   * @param default_value Value to use key wasn't found
   * @param pSection      The section to read the value in
   * @param pKey          The key whose value needs to be read out
   *
   * @return FALSE if could not read the value out of the profile
   * (then the value is not changed)
   */
  bool ReadUintIntPairValue(std::pair<uint32_t, int32_t>* value,
                            const std::pair<uint32_t, uint32_t>& default_value,
                            const char* const pSection,
                            const char* const pKey) const;

  /**
   * @brief Reads an uint16/32/64_t value from the profile
   *
   * @param value         Result value
   * @param default_value Value to use key wasn't found
   * @param pSection      The section to read the value in
   * @param pKey          The key whose value needs to be read out
   *
   * @return FALSE if could not read the value out of the profile
   * (then the value is changed to default)
   */
  bool ReadUIntValue(uint16_t* value,
                     uint16_t default_value,
                     const char* const pSection,
                     const char* const pKey) const;

  bool ReadUIntValue(uint32_t* value,
                     uint32_t default_value,
                     const char* const pSection,
                     const char* const pKey) const;

  bool ReadUIntValue(uint64_t* value,
                     uint64_t default_value,
                     const char* const pSection,
                     const char* const pKey) const;

  /**
   * @brief Checks, if path is relative
   * @param path Path
   * @return true, if is relative, otherwise - false
   */
  bool IsRelativePath(const std::string& path);

  /**
   * @brief Makes relative path absolute
   * @param path Path
   */
  void MakeAbsolutePath(std::string& path);

  /**
   * @brief Converts input string to number
   * @param input Input string
   * @param output Output number
   * @return true, if successfully converted, otherwise - false
   */
  bool StringToNumber(const std::string& input, uint64_t& output) const;

 private:
  std::string sdl_version_;
  bool launch_hmi_;
#ifdef WEB_HMI
  std::string link_to_web_hmi_;
#endif  // WEB_HMI
  std::string app_config_folder_;
  std::string app_storage_folder_;
  std::string app_resource_folder_;
  std::string app_icons_folder_;
  uint32_t app_icons_folder_max_size_;
  uint32_t app_icons_amount_to_remove_;
  size_t maximum_control_payload_size_;
  size_t maximum_rpc_payload_size_;
  size_t maximum_audio_payload_size_;
  size_t maximum_video_payload_size_;
  std::string config_file_name_;
  std::string server_address_;
  uint16_t server_port_;
  uint16_t video_streaming_port_;
  uint16_t audio_streaming_port_;
  uint32_t stop_streaming_timeout_;
  uint16_t time_testing_port_;
  std::string hmi_capabilities_file_name_;
  std::vector<std::string> help_prompt_;
  std::vector<std::string> time_out_promt_;
  std::vector<std::string> vr_commands_;
  uint64_t min_tread_stack_size_;
  bool is_mixing_audio_supported_;
  bool is_redecoding_enabled_;
  uint32_t max_cmd_id_;
  uint32_t default_timeout_;
  uint32_t app_resuming_timeout_;
  uint32_t app_resumption_save_persistent_data_timeout_;
  std::string vr_help_title_;
  uint32_t app_dir_quota_;
  std::string video_consumer_type_;
  std::string audio_consumer_type_;
  std::string named_video_pipe_path_;
  std::string named_audio_pipe_path_;
  uint32_t app_hmi_level_none_time_scale_max_requests_;
  uint32_t app_hmi_level_none_requests_time_scale_;
  std::string video_stream_file_;
  std::string audio_stream_file_;
  uint32_t app_time_scale_max_requests_;
  uint32_t app_requests_time_scale_;
  uint32_t pending_requests_amount_;
  uint32_t put_file_in_none_;
  uint32_t delete_file_in_none_;
  uint32_t list_files_in_none_;
  uint32_t list_files_response_size_;
  std::string app_info_storage_;
  uint32_t heart_beat_timeout_;
  uint16_t max_supported_protocol_version_;
  std::string preloaded_pt_file_;
  std::string policy_snapshot_file_name_;
  bool enable_policy_;
  uint32_t transport_manager_disconnect_timeout_;
  bool use_last_state_;
  std::vector<uint32_t> supported_diag_modes_;
  std::string system_files_path_;
  uint16_t transport_manager_tcp_adapter_port_;
  std::string transport_manager_tcp_adapter_network_interface_;
  std::string tts_delimiter_;
  uint32_t audio_data_stopped_timeout_;
  uint32_t video_data_stopped_timeout_;
  std::string mme_db_name_;
  std::string event_mq_name_;
  std::string ack_mq_name_;
  std::string recording_file_source_;
  std::string recording_file_name_;
  uint32_t application_list_update_timeout_;
  uint32_t max_thread_pool_size_;
  uint32_t default_hub_protocol_index_;
#ifdef ENABLE_SECURITY
  std::string cert_path_;
  std::string ca_cert_path_;
  std::string ssl_mode_;
  std::string key_path_;
  std::string ciphers_list_;
  bool verify_peer_;
  uint32_t update_before_hours_;
  std::string security_manager_protocol_name_;
  std::vector<int> force_protected_service_;
  std::vector<int> force_unprotected_service_;
#endif

  /*
   * first value is count of request
   * second is time scale
   */
  std::pair<uint32_t, int32_t> read_did_frequency_;

  /*
   * first value is count of request
   * second is time scale
   */
  std::pair<uint32_t, int32_t> get_vehicle_data_frequency_;

  /**
   * first value is count of retries for start stream
   * second for timer
   */
  std::pair<uint32_t, int32_t> start_stream_retry_amount_;

  std::string iap_legacy_protocol_mask_;
  std::string iap_hub_protocol_mask_;
  std::string iap_pool_protocol_mask_;
  std::string iap_system_config_;
  std::string iap2_system_config_;
  std::string plugins_folder_;
  int iap2_hub_connect_attempts_;
  int iap_hub_connection_wait_timeout_;
  uint16_t tts_global_properties_timeout_;
  uint16_t attempts_to_open_policy_db_;
  uint16_t open_attempt_timeout_ms_;
  uint32_t resumption_delay_before_ign_;
  uint32_t resumption_delay_after_ign_;
  uint32_t hash_string_size_;
  bool logs_enabled_;
  bool use_db_for_resumption_;
  uint16_t attempts_to_open_resumption_db_;
  uint16_t open_attempt_timeout_ms_resumption_db_;
  std::map<std::string, std::vector<std::string> >
      transport_required_for_resumption_map_;
  std::string navigation_lowbandwidth_resumption_level_;
  std::string projection_lowbandwidth_resumption_level_;
  std::string mediaapps_lowbandwidth_resumption_level_;
  uint16_t app_launch_wait_time_;
  uint16_t app_launch_max_retry_attempt_;
  uint16_t app_launch_retry_wait_time_;
  uint16_t remove_bundle_id_attempts_;
  uint16_t max_number_of_ios_device_;
  uint16_t wait_time_between_apps_;
  bool enable_app_launch_ios_;
  uint32_t app_tranport_change_timer_;
  uint32_t app_tranport_change_timer_addition_;
  bool multiple_transports_enabled_;
  std::vector<std::string> secondary_transports_for_bluetooth_;
  std::vector<std::string> secondary_transports_for_usb_;
  std::vector<std::string> secondary_transports_for_wifi_;
  std::vector<std::string> audio_service_transports_;
  std::vector<std::string> video_service_transports_;
  bool error_occured_;
  std::string error_description_;

  DISALLOW_COPY_AND_ASSIGN(Profile);
};
}  //  namespace profile

#endif  // SRC_COMPONENTS_CONFIG_PROFILE_INCLUDE_CONFIG_PROFILE_PROFILE_H_
