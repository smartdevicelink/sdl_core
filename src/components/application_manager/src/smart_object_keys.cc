#include <application_manager/smart_object_keys.h>

namespace application_manager {

namespace strings {

const char* params = "params";
const char* require_encryption = "requireEncryption";
const char* protection = "protection";
const char* message_type = "message_type";
const char* correlation_id = "correlation_id";
const char* function_id = "function_id";
const char* protocol_version = "protocol_version";
const char* protocol_type = "protocol_type";
const char* connection_key = "connection_key";
const char* error = "error";
const char* error_msg = "message";
const char* default_app_id = "default";
const char* msg_params = "msg_params";
const char* method_name = "methodName";
const char* info = "info";
const char* app_id = "appID";
const char* full_app_id = "fullAppID";
const char* bundle_id = "appBundleID";
const char* window_id = "windowID";
const char* app_info = "appInfo";
const char* app_launch = "app_launch";
const char* app_launch_list = "app_launch_list";
const char* app_launch_last_session = "app_launch_last_session";
const char* policy_app_id = "policyAppID";
const char* hmi_app_id = "hmiAppID";
const char* device_id = "deviceID";
const char* subscribe = "subscribe";
const char* subscribed_for_way_points = "subscribed_for_way_points";
const char* url = "url";
const char* urlScheme = "urlScheme";
const char* packageName = "packageName";
const char* cmd_icon = "cmdIcon";
const char* result_code = "resultCode";
const char* success = "success";
const char* sync_msg_version = "syncMsgVersion";
const char* major_version = "majorVersion";
const char* minor_version = "minorVersion";
const char* patch_version = "patchVersion";
const char* app_name = "appName";
const char* ngn_media_screen_app_name = "ngnMediaScreenAppName";
const char* vr_synonyms = "vrSynonyms";
const char* uses_vehicle_data = "usesVehicleData";
const char* is_media_application = "isMediaApplication";
const char* grey_out = "greyOut";
const char* language_desired = "languageDesired";
const char* auto_activated_id = "autoActivateID";
const char* app_type = "appType";
const char* app_hmi_type = "appHMIType";
const char* tts_name = "ttsName";
const char* binary_data = "binary_data";
const char* timeout_prompt = "timeoutPrompt";
const char* timeout = "timeout";
const char* vr_help_title = "vrHelpTitle";
const char* vr_help = "vrHelp";
const char* main_field_1 = "mainField1";
const char* main_field_2 = "mainField2";
const char* main_field_3 = "mainField3";
const char* main_field_4 = "mainField4";
const char* template_title = "templateTitle";
const char* metadata_tags = "metadataTags";
const char* eta = "eta";
const char* time_to_destination = "timeToDestination";
const char* total_distance = "totalDistance";
const char* alignment = "alignment";
const char* graphic = "graphic";
const char* secondary_graphic = "secondaryGraphic";
const char* status_bar = "statusBar";
const char* media_clock = "mediaClock";
const char* media_track = "mediaTrack";
const char* properties = "properties";
const char* cmd_id = "cmdID";
const char* menu_params = "menuParams";
const char* menu_title = "menuTitle";
const char* menu_icon = "menuIcon";
const char* keyboard_properties = "keyboardProperties";
const char* vr_commands = "vrCommands";
const char* position = "position";
const char* user_location = "userLocation";
const char* num_ticks = "numTicks";
const char* slider_footer = "sliderFooter";
const char* menu_id = "menuID";
const char* parent_id = "parentID";
const char* menu_name = "menuName";
const char* menu_layout = "menuLayout";
const char* menu_layouts_available = "menuLayoutsAvailable";
const char* interaction_choice_set_id = "interactionChoiceSetID";
const char* interaction_choice_set_id_list = "interactionChoiceSetIDList";
const char* choice_set = "choiceSet";
const char* choice_id = "choiceID";
const char* grammar_id = "grammarID";
const char* navigation_text_1 = "navigationText1";
const char* navigation_text_2 = "navigationText2";
const char* alert_text1 = "alertText1";
const char* alert_text2 = "alertText2";
const char* alert_text3 = "alertText3";
const char* tts_chunks = "ttsChunks";
const char* initial_prompt = "initialPrompt";
const char* initial_text = "initialText";
const char* duration = "duration";
const char* progress_indicator = "progressIndicator";
const char* func_id = "functionID";
const char* cancel_id = "cancelID";
const char* alert_type = "alertType";
const char* alert_icon = "alertIcon";
const char* play_tone = "playTone";
const char* soft_buttons = "softButtons";
const char* soft_button_id = "softButtonID";
const char* custom_presets = "customPresets";
const char* audio_pass_display_text1 = "audioPassThruDisplayText1";
const char* audio_pass_display_text2 = "audioPassThruDisplayText2";
const char* max_duration = "maxDuration";
const char* sampling_rate = "samplingRate";
const char* bits_per_sample = "bitsPerSample";
const char* audio_type = "audioType";
const char* mute_audio = "muteAudio";
const char* button_name = "buttonName";
const char* button_event_mode = "buttonEventMode";
const char* button_press_mode = "buttonPressMode";
const char* custom_button_id = "customButtonID";
const char* data_type = "dataType";
const char* turn_list = "turnList";
const char* turn_icon = "turnIcon";
const char* next_turn_icon = "nextTurnIcon";
const char* value = "value";
const char* hmi_display_language = "hmiDisplayLanguage";
const char* language = "language";
const char* data = "data";
const char* start_time = "startTime";
const char* end_time = "endTime";
const char* hours = "hours";
const char* minutes = "minutes";
const char* seconds = "seconds";
const char* update_mode = "updateMode";
const char* audioStreamingIndicator = "audioStreamingIndicator";
const char* seek_time = "seekTime";
const char* forward_seek_indicator = "forwardSeekIndicator";
const char* back_seek_indicator = "backSeekIndicator";
const char* trigger_source = "triggerSource";
const char* hmi_level = "hmiLevel";
const char* activate_app_hmi_level = "level";
const char* audio_streaming_state = "audioStreamingState";
const char* video_streaming_state = "videoStreamingState";
const char* system_context = "systemContext";
const char* window_name = "windowName";
const char* window_type = "type";
const char* window_type_supported = "windowTypeSupported";
const char* maximum_number_of_windows = "maximumNumberOfWindows";
const char* window_capabilities = "windowCapabilities";
const char* associated_service_type = "associatedServiceType";
const char* duplicate_updates_from_window_id = "duplicateUpdatesFromWindowID";
const char* speech_capabilities = "speechCapabilities";
const char* vr_capabilities = "vrCapabilities";
const char* audio_pass_thru_capabilities = "audioPassThruCapabilities";
const char* audio_pass_thru_capabilities_list = "audioPassThruCapabilitiesList";
const char* pcm_stream_capabilities = "pcmStreamCapabilities";
const char* audio_pass_thru_icon = "audioPassThruIcon";
const char* way_points = "wayPoints";
const char* system_capability = "systemCapability";
const char* system_capability_type = "systemCapabilityType";
const char* system_capabilities = "systemCapabilities";
const char* navigation_capability = "navigationCapability";
const char* phone_capability = "phoneCapability";
const char* video_streaming_capability = "videoStreamingCapability";
const char* driver_distraction_capability = "driverDistractionCapability";
const char* driver_distraction = "driverDistraction";
const char* rc_capability = "remoteControlCapability";
const char* seat_location_capability = "seatLocationCapability";
const char* app_services_capabilities = "appServicesCapabilities";
const char* day_color_scheme = "dayColorScheme";
const char* night_color_scheme = "nightColorScheme";
const char* primary_color = "primaryColor";
const char* secondary_color = "secondaryColor";
const char* background_color = "backgroundColor";
const char* red = "red";
const char* green = "green";
const char* blue = "blue";
const char* display_layout = "displayLayout";
const char* template_configuration = "templateConfiguration";
const char* template_layout = "template";
const char* icon_resumed = "iconResumed";
const char* nicknames = "nicknames";
const char* enabled = "enabled";
const char* auth_token = "authToken";
const char* cloud_transport_type = "cloudTransportType";
const char* hybrid_app_preference = "hybridAppPreference";
const char* is_cloud_application = "isCloudApplication";
const char* cloud_connection_status = "cloudConnectionStatus";
const char* endpoint = "endpoint";
const char* display_capabilities = "displayCapabilities";
const char* policy_type = "policyType";
const char* property = "property";
const char* displays = "displays";
const char* seat_location = "seatLocation";
const char* app_capability = "appCapability";
const char* app_capability_type = "appCapabilityType";

// PutFile
const char* sync_file_name = "syncFileName";
const char* file_name = "fileName";
const char* file_path = "filePath";
const char* file_type = "fileType";
const char* file_size = "fileSize";
const char* crc32_check_sum = "crc";
const char* request_type = "requestType";
const char* request_subtype = "requestSubType";
const char* persistent_file = "persistentFile";
const char* file_data = "fileData";
const char* space_available = "spaceAvailable";
const char* image_type = "imageType";
const char* is_template = "isTemplate";
const char* image = "image";
const char* type = "type";
const char* system_file = "systemFile";
const char* is_system_file = "isSystemFile";
const char* offset = "offset";
const char* length = "length";
const char* secondary_image = "secondaryImage";
const char* filenames = "filenames";

const char* hmi_display_language_desired = "hmiDisplayLanguageDesired";
const char* ecu_name = "ecuName";
const char* dtc_mask = "dtcMask";
const char* did_location = "didLocation";
const char* app_list = "appList";
const char* device_list = "deviceList";
const char* device_info = "deviceInfo";
const char* secondary_device_info = "secondaryDeviceInfo";
const char* name = "name";
const char* id = "id";
const char* isSDLAllowed = "isSDLAllowed";
const char* transport_type = "transportType";
const char* application = "application";
const char* applications = "applications";
const char* icon = "icon";
const char* device_name = "deviceName";
const char* reason = "reason";
const char* available = "available";
const char* text = "text";
const char* character_set = "characterSet";
const char* secondary_text = "secondaryText";
const char* tertiary_text = "tertiaryText";
const char* hardware = "hardware";
const char* firmware_rev = "firmwareRev";
const char* os = "os";
const char* os_version = "osVersion";
const char* carrier = "carrier";
const char* slider_header = "sliderHeader";
const char* key_press_mode = "keypressMode";

// duplicate names from hmi_request
const char* limited_character_list = "limitedCharacterList";
const char* auto_complete_text = "autoCompleteText";
const char* auto_complete_list = "autoCompleteList";
const char* navigation_text = "navigationText";

// vehicle info
const char* gps = "gps";
const char* speed = "speed";
const char* rpm = "rpm";
const char* fuel_level = "fuelLevel";
const char* fuel_level_state = "fuelLevel_State";
const char* instant_fuel_consumption = "instantFuelConsumption";
const char* fuel_range = "fuelRange";
const char* cloud_app_vehicle_id = "cloudAppVehicleID";
const char* climate_data = "climateData";
const char* external_temp = "externalTemperature";
const char* turn_signal = "turnSignal";
const char* vin = "vin";
const char* gearStatus = "gearStatus";
const char* prndl = "prndl";
const char* tire_pressure = "tirePressure";
const char* pressure_telltale = "pressureTelltale";
const char* left_front = "leftFront";
const char* right_front = "rightFront";
const char* left_rear = "leftRear";
const char* right_rear = "rightRear";
const char* inner_left_rear = "innerLeftRear";
const char* inner_right_rear = "innerRightRear";
const char* odometer = "odometer";
const char* belt_status = "beltStatus";
const char* electronic_park_brake_status = "electronicParkBrakeStatus";
const char* body_information = "bodyInformation";
const char* device_status = "deviceStatus";
const char* driver_braking = "driverBraking";
const char* wiper_status = "wiperStatus";
const char* head_lamp_status = "headLampStatus";
const char* engine_torque = "engineTorque";
const char* acc_pedal_pos = "accPedalPosition";
const char* steering_wheel_angle = "steeringWheelAngle";
const char* stability_controls_status = "stabilityControlsStatus";
const char* e_call_info = "eCallInfo";
const char* airbag_status = "airbagStatus";
const char* emergency_event = "emergencyEvent";
const char* cluster_mode_status = "clusterModeStatus";
const char* cluster_modes = "clusterModes";
const char* my_key = "myKey";
const char* help_prompt = "helpPrompt";
const char* scroll_message_body = "scrollableMessageBody";
const char* data_result = "dataResult";
const char* dtc_list = "dtcList";
const char* interaction_mode = "interactionMode";
const char* slider_position = "sliderPosition";
const char* system_action = "systemAction";
const char* prerecorded_speech = "prerecordedSpeech";
const char* supported_diag_modes = "supportedDiagModes";
const char* hmi_capabilities = "hmiCapabilities";
const char* navigation = "navigation";
const char* phone_call = "phoneCall";
const char* video_streaming = "videoStreaming";
const char* remote_control = "remoteControl";
const char* sdl_version = "sdlVersion";
const char* system_software_version = "systemSoftwareVersion";
const char* system_hardware_version = "systemHardwareVersion";
const char* priority = "priority";
const char* engine_oil_life = "engineOilLife";
const char* oem_custom_data_type = "oemCustomDataType";
const char* window_status = "windowStatus";
const char* hands_off_steering = "handsOffSteering";
const char* seat_occupancy = "seatOccupancy";

// app services
const char* app_service_manifest = "appServiceManifest";
const char* service_name = "serviceName";
const char* service_type = "serviceType";
const char* service_icon = "serviceIcon";
const char* allow_app_consumers = "allowAppConsumers";
const char* rpc_spec_version = "rpcSpecVersion";
const char* handled_rpcs = "handledRPCs";
const char* media_service_manifest = "mediaServiceManifest";
const char* app_service_record = "appServiceRecord";
const char* service_id = "serviceID";
const char* service_manifest = "serviceManifest";
const char* service_published = "servicePublished";
const char* service_active = "serviceActive";
const char* app_service_id = "appServiceId";
const char* service_data = "serviceData";
const char* media_service_data = "mediaServiceData";
const char* media_image = "mediaImage";
const char* weather_service_data = "weatherServiceData";
const char* location = "location";
const char* current_forecast = "currentForecast";
const char* minute_forecast = "minuteForecast";
const char* hourly_forecast = "hourlyForecast";
const char* multiday_forecast = "multidayForecast";
const char* weather_icon = "weatherIcon";
const char* navigation_service_data = "navigationServiceData";
const char* origin = "origin";
const char* destination = "destination";
const char* instructions = "instructions";
const char* location_details = "locationDetails";
const char* request_service_active = "requestServiceActive";
const char* app_services = "appServices";
const char* update_reason = "updateReason";
const char* updated_app_service_record = "updatedAppServiceRecord";
const char* service_records = "serviceRecords";
const char* activate = "activate";
const char* set_as_default = "setAsDefault";
const char* origin_app = "originApp";

// sis data
const char* station_short_name = "stationShortName";
const char* station_location = "stationLocation";

// resuming
const char* application_commands = "applicationCommands";
const char* application_submenus = "applicationSubMenus";
const char* application_choice_sets = "applicationChoiceSets";
const char* application_global_properties = "globalProperties";
const char* application_vehicle_info = "vehicleInfo";
const char* application_buttons = "buttons";
const char* application_subscriptions = "subscriptions";
const char* application_files = "applicationFiles";
const char* application_show = "applicationShow";
const char* resumption = "resumption";
const char* resume_app_list = "resume_app_list";
const char* last_ign_off_time = "last_ign_off_time";

const char* resume_vr_grammars = "resumeVrGrammars";

const char* ign_off_count = "ign_off_count";
const char* global_ign_on_counter = "global_ign_on_counter";
const char* suspend_count = "suspend_count";

const char* connection_info = "connection_info";
const char* is_download_complete = "is_download_complete";

const char* hash_id = "hashID";
const char* time_stamp = "timeStamp";
const char* manual_text_entry = "manualTextEntry";
const char* image_type_supported = "imageTypeSupported";
const char* unexpected_disconnect = "unexpectedDisconnect";

const char* shifted = "shifted";
const char* altitude = "altitude";
const char* longitude_degrees = "longitudeDegrees";
const char* latitude_degrees = "latitudeDegrees";

const char* address = "address";
const char* country_name = "countryName";
const char* country_code = "countryCode";
const char* postal_code = "postalCode";
const char* administrative_area = "administrativeArea";
const char* locality = "locality";
const char* sub_locality = "subLocality";
const char* thoroughfare = "thoroughfare";
const char* sub_thoroughfare = "subThoroughfare";
const char* location_name = "locationName";
const char* location_description = "locationDescription";
const char* address_lines = "addressLines";
const char* phone_number = "phoneNumber";
const char* number = "number";
const char* location_image = "locationImage";
const char* is_suscribed = "isSubscribed";
const char* message_data = "messageData";

const char* delivery_mode = "deliveryMode";

const char* audio_streaming_indicator = "audioStreamingIndicator";

const char* windows_info = "windowsInfo";

const char* const keyboard_properties_supported = "keyboardPropertiesSupported";
const char* const language_supported = "languageSupported";
const char* const keyboard_layout_supported = "keyboardLayoutSupported";
const char* const keypress_mode_supported = "keypressModeSupported";
const char* const limited_characters_list_supported =
    "limitedCharactersListSupported";
const char* const send_location_enabled = "sendLocationEnabled";
const char* const get_way_points_enabled = "getWayPointsEnabled";
const char* const entity_type = "entityType";
const char* const entity_id = "entityID";
const char* const status = "status";
const char* const external_consent_status = "externalConsentStatus";
const char* const consented_functions = "consentedFunctions";
const char* const source = "source";
const char* const config = "config";
const char* const protocol = "protocol";
const char* const codec = "codec";
const char* const width = "width";
const char* const height = "height";
const char* const rejected_params = "rejectedParams";
const char* const preferred_resolution = "preferredResolution";
const char* const resolution_width = "resolutionWidth";
const char* const resolution_height = "resolutionHeight";
const char* const max_bitrate = "maxBitrate";
const char* const supported_formats = "supportedFormats";
const char* const haptic_spatial_data_supported = "hapticSpatialDataSupported";
const char* const diagonal_screen_size = "diagonalScreenSize";
const char* const pixel_per_inch = "pixelPerInch";
const char* const scale = "scale";
const char* const additional_video_streaming_capabilities =
    "additionalVideoStreamingCapabilities";
const char* const haptic_rect_data = "hapticRectData";
const char* const rect = "rect";
const char* const x = "x";
const char* const y = "y";
const char* const preferred_fps = "preferredFPS";

// OnResetTimeout
const char* const request_id = "requestID";
const char* const reset_period = "resetPeriod";
}  // namespace strings

namespace hmi_interface {
const char* basic_communication = "BasicCommunication";
const char* buttons = "Buttons";
const char* navigation = "Navigation";
const char* sdl = "SDL";
const char* tts = "TTS";
const char* ui = "UI";
const char* vr = "VR";
const char* rc = "RC";
const char* vehicle_info = "VehicleInfo";
const char* app_service = "AppService";
}  // namespace hmi_interface

namespace json {
const char* appId = "appId";
const char* name = "name";
const char* ios = "ios";
const char* android = "android";
const char* appHmiType = "appHmiType";
const char* urlScheme = "urlScheme";
const char* packageName = "packageName";
const char* response = "response";
const char* is_media_application = "isMediaApplication";
const char* default_ = "default";
const char* languages = "languages";
const char* ttsName = "ttsName";
const char* vrSynonyms = "vrSynonyms";
}  // namespace json

namespace http_request {
const char* httpRequest = "HTTPRequest";
const char* headers = "headers";
const char* content_type = "ContentType";
const char* connect_timeout = "ConnectTimout";
const char* do_output = "DoOutput";
const char* do_input = "DoInput";
const char* use_caches = "UseCaches";
const char* request_method = "RequestMethod";
const char* read_timeout = "ReadTimeout";
const char* instance_follow_redirect = "InstanceFollowRedirect";
const char* charset = "charset";
const char* content_lenght = "Content_Lenght";
const char* GET = "GET";
}  // namespace http_request

namespace mobile_notification {
const char* state = "state";
const char* syncp_timeout = "Timeout";
const char* syncp_url = "URL";
const char* lock_screen_dismissal_enabled = "lockScreenDismissalEnabled";
const char* lock_screen_dismissal_warning = "lockScreenDismissalWarning";
}  // namespace mobile_notification

namespace hmi_levels {
const char* kFull = "FULL";
const char* kLimited = "LIMITED";
const char* kBackground = "BACKGROUND";
const char* kNone = "NONE";
}  // namespace hmi_levels

namespace time_keys {
const char* millisecond = "millisecond";
const char* second = "second";
const char* minute = "minute";
const char* hour = "hour";
const char* day = "day";
const char* month = "month";
const char* year = "year";
}  // namespace time_keys

namespace hmi_request {
const char* parent_id = "parentID";
const char* field_name = "fieldName";
const char* field_text = "fieldText";
const char* field_types = "fieldTypes";
const char* alert_strings = "alertStrings";
const char* duration = "duration";
const char* soft_buttons = "softButtons";
const char* tts_chunks = "ttsChunks";
const char* speak_type = "speakType";
const char* audio_pass_display_texts = "audioPassThruDisplayTexts";
const char* max_duration = "maxDuration";
const char* reason = "reason";
const char* message_text = "messageText";
const char* initial_text = "initialText";
const char* navi_texts = "navigationTexts";
const char* navi_text = "navigationText";
const char* show_strings = "showStrings";
const char* interaction_layout = "interactionLayout";
const char* menu_title = "menuTitle";
const char* menu_icon = "menuIcon";
const char* keyboard_properties = "keyboardProperties";
const char* method_name = "methodName";
const char* keyboard_layout = "keyboardLayout";
const char* limited_character_list = "limitedCharacterList";
const char* auto_complete_list = "autoCompleteList";
const char* mask_input_characters = "maskInputCharacters";
const char* custom_keys = "customKeys";
const char* file = "file";
const char* file_name = "fileName";
const char* retry = "retry";
const char* service = "service";
}  // namespace hmi_request

namespace hmi_response {
const char* code = "code";
const char* message = "message";
const char* method = "method";
const char* try_again_time = "tryAgainTime";
const char* custom_button_id = "customButtonID";
const char* button_name = "name";
const char* button_mode = "mode";
const char* attenuated_supported = "attenuatedSupported";
const char* languages = "languages";
const char* language = "language";
const char* display_capabilities = "displayCapabilities";
const char* hmi_zone_capabilities = "hmiZoneCapabilities";
const char* soft_button_capabilities = "softButtonCapabilities";
const char* image_supported = "imageSupported";
const char* button_capabilities = "buttonCapabilities";
const char* capabilities = "capabilities";
const char* speech_capabilities = "speechCapabilities";
const char* prerecorded_speech_capabilities = "prerecordedSpeechCapabilities";
const char* preset_bank_capabilities = "presetBankCapabilities";
const char* on_screen_presets_available = "onScreenPresetsAvailable";
const char* allowed = "allowed";
const char* vehicle_type = "vehicleType";
const char* did_result = "didResult";
const char* result_code = "resultCode";
const char* dtc = "dtc";
const char* ecu_header = "ecuHeader";
const char* image_capabilities = "imageCapabilities";
const char* display_type = "displayType";
const char* display_name = "displayName";
const char* text_fields = "textFields";
const char* keyboard_capabilities = "keyboardCapabilities";
const char* supported_keyboards = "supportedKeyboards";
const char* num_configurable_keys = "numConfigurableKeys";
const char* media_clock_formats = "mediaClockFormats";
const char* graphic_supported = "graphicSupported";
const char* image_fields = "imageFields";
const char* templates_available = "templatesAvailable";
const char* screen_params = "screenParams";
const char* num_custom_presets_available = "numCustomPresetsAvailable";
const char* urls = "urls";
const char* policy_app_id = "policyAppID";
const char* enabled = "enabled";
const char* system_time = "systemTime";
}  // namespace hmi_response

namespace hmi_notification {
const char* prndl = "prndl";
const char* file_name = "file_name";
const char* system_context = "systemContext";
const char* state = "state";
const char* result = "result";
const char* statistic_type = "statisticType";
const char* error = "error";
const char* policyfile = "policyfile";
const char* is_active = "isActive";
const char* is_deactivated = "isDeactivated";
const char* event_name = "eventName";
const char* service_type = "serviceType";
const char* service_event = "serviceEvent";
const char* reason = "reason";
}  // namespace hmi_notification

}  // namespace application_manager
