/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Copyright (c) 2017 Xevo Inc.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the copyright holders nor the names of their contributors
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SMART_OBJECT_KEYS_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SMART_OBJECT_KEYS_H_

namespace application_manager {

namespace strings {

extern const char* params;
extern const char* message_type;
extern const char* correlation_id;
extern const char* function_id;
extern const char* protocol_version;
extern const char* protocol_type;
extern const char* connection_key;
extern const char* error;
extern const char* error_msg;
extern const char* default_app_id;
extern const char* msg_params;
extern const char* method_name;
extern const char* info;
extern const char* app_id;
extern const char* bundle_id;
extern const char* app_info;
extern const char* app_launch;
extern const char* app_launch_list;
extern const char* app_launch_last_session;
extern const char* policy_app_id;
extern const char* hmi_app_id;
extern const char* device_id;
extern const char* subscribed_for_way_points;
extern const char* url;
extern const char* urlScheme;
extern const char* packageName;
extern const char* cmd_icon;
extern const char* result_code;
extern const char* success;
extern const char* sync_msg_version;
extern const char* major_version;
extern const char* minor_version;
extern const char* patch_version;
extern const char* app_name;
extern const char* ngn_media_screen_app_name;
extern const char* vr_synonyms;
extern const char* uses_vehicle_data;
extern const char* is_media_application;
extern const char* greyOut;
extern const char* language_desired;
extern const char* auto_activated_id;
extern const char* app_type;
extern const char* app_hmi_type;
extern const char* tts_name;
extern const char* binary_data;
extern const char* timeout_prompt;
extern const char* timeout;
extern const char* vr_help_title;
extern const char* vr_help;
extern const char* main_field_1;
extern const char* main_field_2;
extern const char* main_field_3;
extern const char* main_field_4;
extern const char* metadata_tags;
extern const char* eta;
extern const char* time_to_destination;
extern const char* total_distance;
extern const char* alignment;
extern const char* graphic;
extern const char* secondary_graphic;
extern const char* status_bar;
extern const char* media_clock;
extern const char* media_track;
extern const char* properties;
extern const char* cmd_id;
extern const char* menu_params;
extern const char* menu_title;
extern const char* menu_icon;
extern const char* keyboard_properties;
extern const char* vr_commands;
extern const char* position;
extern const char* num_ticks;
extern const char* slider_footer;
extern const char* menu_id;
extern const char* menu_name;
extern const char* interaction_choice_set_id;
extern const char* interaction_choice_set_id_list;
extern const char* choice_set;
extern const char* choice_id;
extern const char* grammar_id;
extern const char* navigation_text_1;
extern const char* navigation_text_2;
extern const char* alert_text1;
extern const char* alert_text2;
extern const char* alert_text3;
extern const char* tts_chunks;
extern const char* initial_prompt;
extern const char* initial_text;
extern const char* duration;
extern const char* progress_indicator;
extern const char* alert_type;
extern const char* play_tone;
extern const char* soft_buttons;
extern const char* soft_button_id;
extern const char* custom_presets;
extern const char* audio_pass_display_text1;
extern const char* audio_pass_display_text2;
extern const char* max_duration;
extern const char* sampling_rate;
extern const char* bits_per_sample;
extern const char* audio_type;
extern const char* mute_audio;
extern const char* button_name;
extern const char* button_event_mode;
extern const char* button_press_mode;
extern const char* custom_button_id;
extern const char* data_type;
extern const char* turn_list;
extern const char* turn_icon;
extern const char* next_turn_icon;
extern const char* value;
extern const char* hmi_display_language;
extern const char* language;
extern const char* data;
extern const char* start_time;
extern const char* end_time;
extern const char* hours;
extern const char* minutes;
extern const char* seconds;
extern const char* update_mode;
extern const char* trigger_source;
extern const char* hmi_level;
extern const char* activate_app_hmi_level;
extern const char* audio_streaming_state;
extern const char* video_streaming_state;
extern const char* system_context;
extern const char* speech_capabilities;
extern const char* vr_capabilities;
extern const char* audio_pass_thru_capabilities;
extern const char* pcm_stream_capabilities;
extern const char* audio_pass_thru_icon;
extern const char* way_points;
extern const char* system_capability;
extern const char* system_capability_type;
extern const char* system_capabilities;
extern const char* navigation_capability;
extern const char* phone_capability;
extern const char* video_streaming_capability;
extern const char* rc_capability;

// PutFile
extern const char* sync_file_name;
extern const char* file_name;
extern const char* file_type;
extern const char* file_size;
extern const char* request_type;
extern const char* persistent_file;
extern const char* file_data;
extern const char* space_available;
extern const char* image_type;
extern const char* image;
extern const char* type;
extern const char* system_file;
extern const char* offset;
extern const char* length;
extern const char* secondary_image;
extern const char* filenames;

extern const char* hmi_display_language_desired;
extern const char* ecu_name;
extern const char* dtc_mask;
extern const char* did_location;
extern const char* app_list;
extern const char* device_list;
extern const char* device_info;
extern const char* name;
extern const char* id;
extern const char* isSDLAllowed;
extern const char* transport_type;
extern const char* application;
extern const char* applications;
extern const char* icon;
extern const char* device_name;
extern const char* reason;
extern const char* available;
extern const char* text;
extern const char* character_set;
extern const char* secondary_text;
extern const char* tertiary_text;
extern const char* hardware;
extern const char* firmware_rev;
extern const char* os;
extern const char* os_version;
extern const char* carrier;
extern const char* slider_header;
extern const char* key_press_mode;

// duplicate names from hmi_request
extern const char* limited_character_list;
extern const char* auto_complete_text;
extern const char* navigation_text;

// vehicle info
extern const char* gps;
extern const char* speed;
extern const char* rpm;
extern const char* fuel_level;
extern const char* fuel_level_state;
extern const char* instant_fuel_consumption;
extern const char* external_temp;
extern const char* vin;
extern const char* prndl;
extern const char* tire_pressure;
extern const char* odometer;
extern const char* belt_status;
extern const char* body_information;
extern const char* device_status;
extern const char* driver_braking;
extern const char* wiper_status;
extern const char* head_lamp_status;
extern const char* engine_torque;
extern const char* acc_pedal_pos;
extern const char* steering_wheel_angle;
extern const char* e_call_info;
extern const char* airbag_status;
extern const char* emergency_event;
extern const char* cluster_mode_status;
extern const char* my_key;
extern const char* help_prompt;
extern const char* scroll_message_body;
extern const char* data_result;
extern const char* dtc_list;
extern const char* interaction_mode;
extern const char* slider_position;
extern const char* system_action;
extern const char* prerecorded_speech;
extern const char* supported_diag_modes;
extern const char* hmi_capabilities;
extern const char* navigation;
extern const char* phone_call;
extern const char* video_streaming;
extern const char* remote_control;
extern const char* sdl_version;
extern const char* system_software_version;
extern const char* priority;

// resuming
extern const char* application_commands;
extern const char* application_submenus;
extern const char* application_choice_sets;
extern const char* application_global_properties;
extern const char* application_vehicle_info;
extern const char* application_buttons;
extern const char* application_subscribtions;
extern const char* application_files;
extern const char* application_show;
extern const char* resumption;
extern const char* resume_app_list;
extern const char* last_ign_off_time;

extern const char* resume_vr_grammars;

extern const char* ign_off_count;

extern const char* connection_info;
extern const char* is_download_complete;

extern const char* longitude_degrees;
extern const char* latitude_degrees;
extern const char* address;
extern const char* country_name;
extern const char* country_code;
extern const char* postal_code;
extern const char* administrative_area;
extern const char* locality;
extern const char* sub_locality;
extern const char* thoroughfare;
extern const char* sub_thoroughfare;

extern const char* hash_id;
extern const char* time_stamp;
extern const char* manual_text_entry;
extern const char* image_type_supported;
extern const char* unexpected_disconnect;
extern const char* location_name;
extern const char* location_description;
extern const char* address_lines;
extern const char* phone_number;
extern const char* number;
extern const char* location_image;
extern const char* is_suscribed;
extern const char* message_data;

extern const char* delivery_mode;

extern const char* audio_streaming_indicator;

// keys for default parameters loaded from hmi_capabilities.json:
extern const char* const keyboard_properties_default;
extern const char* const language_default;
extern const char* const keyboard_layout_default;
extern const char* const keypress_mode_default;

// keys for supported parameters loaded from hmi_capabilities.json:
extern const char* const keyboard_properties_supported;
extern const char* const language_supported;
extern const char* const keyboard_layout_supported;
extern const char* const keypress_mode_supported;
extern const char* const limited_characters_list_supported;
extern const char* const auto_complete_text_supported;
extern const char* const entity_type;
extern const char* const entity_id;
extern const char* const status;
extern const char* const external_consent_status;
extern const char* const consented_functions;
extern const char* const source;
extern const char* const config;
extern const char* const protocol;
extern const char* const codec;
extern const char* const width;
extern const char* const height;
extern const char* const rejected_params;
extern const char* const preferred_resolution;
extern const char* const resolution_width;
extern const char* const resolution_height;
extern const char* const max_bitrate;
extern const char* const supported_formats;
extern const char* const haptic_spatial_data_supported;
extern const char* const haptic_rect_data;
extern const char* const rect;
extern const char* const x;
extern const char* const y;
}  // namespace strings

namespace json {
extern const char* appId;
extern const char* name;
extern const char* ios;
extern const char* android;
extern const char* appHmiType;
extern const char* urlScheme;
extern const char* packageName;
extern const char* response;
extern const char* is_media_application;
extern const char* default_;
extern const char* languages;
extern const char* ttsName;
extern const char* vrSynonyms;
}  // namespace json

namespace http_request {
extern const char* httpRequest;
extern const char* headers;
extern const char* content_type;
extern const char* connect_timeout;
extern const char* do_output;
extern const char* do_input;
extern const char* use_caches;
extern const char* request_method;
extern const char* read_timeout;
extern const char* instance_follow_redirect;
extern const char* charset;
extern const char* content_lenght;
extern const char* GET;
}  // http_request

namespace mobile_notification {
extern const char* state;
extern const char* syncp_timeout;
extern const char* syncp_url;
}  // namespace mobile_notification

namespace hmi_levels {
extern const char* kFull;
extern const char* kLimited;
extern const char* kBackground;
extern const char* kNone;
}

namespace hmi_request {
extern const char* parent_id;
extern const char* field_name;
extern const char* field_text;
extern const char* field_types;
extern const char* alert_strings;
extern const char* duration;
extern const char* soft_buttons;
extern const char* tts_chunks;
extern const char* speak_type;
extern const char* audio_pass_display_texts;
extern const char* max_duration;
extern const char* reason;
extern const char* message_text;
extern const char* initial_text;
extern const char* navi_texts;
extern const char* navi_text;
extern const char* show_strings;
extern const char* interaction_layout;
extern const char* menu_title;
extern const char* menu_icon;
extern const char* keyboard_properties;
extern const char* method_name;
extern const char* keyboard_layout;
extern const char* limited_character_list;
extern const char* auto_complete_text;
extern const char* file;
extern const char* file_name;
extern const char* retry;
extern const char* service;
}  // namespace hmi_request

namespace hmi_response {
extern const char* code;
extern const char* message;
extern const char* method;
extern const char* try_again_time;
extern const char* custom_button_id;
extern const char* button_name;
extern const char* button_mode;
extern const char* attenuated_supported;
extern const char* languages;
extern const char* language;
extern const char* display_capabilities;
extern const char* hmi_zone_capabilities;
extern const char* soft_button_capabilities;
extern const char* image_supported;
extern const char* button_capabilities;
extern const char* capabilities;
extern const char* speech_capabilities;
extern const char* prerecorded_speech_capabilities;
extern const char* preset_bank_capabilities;
extern const char* allowed;
extern const char* vehicle_type;
extern const char* did_result;
extern const char* result_code;
extern const char* dtc;
extern const char* ecu_header;
extern const char* image_capabilities;
extern const char* display_type;
extern const char* text_fields;
extern const char* media_clock_formats;
extern const char* graphic_supported;
extern const char* image_fields;
extern const char* templates_available;
extern const char* screen_params;
extern const char* num_custom_presets_available;
extern const char* urls;
extern const char* policy_app_id;
extern const char* enabled;

}  // namespace hmi_response

namespace hmi_notification {
extern const char* prndl;
extern const char* file_name;
extern const char* system_context;
extern const char* state;
extern const char* result;
extern const char* statistic_type;
extern const char* error;
extern const char* policyfile;
extern const char* is_active;
extern const char* is_deactivated;
extern const char* event_name;

}  // namespace hmi_notification

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SMART_OBJECT_KEYS_H_
