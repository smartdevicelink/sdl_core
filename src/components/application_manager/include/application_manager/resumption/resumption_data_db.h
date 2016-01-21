/*
 * Copyright (c) 2015, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_DB_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_DB_H_
#include "application_manager/resumption/resumption_data.h"

#include "sql_database.h"
#include "sql_query.h"

namespace resumption {

/**
 * @brief Points what structure contains vr commands
 */
enum AccessoryVRCommand { kVRCommandFromChoice = 0, kVRCommandFromCommand };

struct ApplicationParams {
  ApplicationParams(const smart_objects::SmartObject& application);
  ApplicationParams(app_mngr::ApplicationSharedPtr application);

  std::string m_hash;
  int64_t m_grammar_id;
  int64_t m_connection_key;
  int64_t m_hmi_app_id;
  mobile_apis::HMILevel::eType m_hmi_level;
  bool m_is_media_application;
  bool m_is_valid;
};

/**
 * @brief class contains logic for representation application data in
 * data base
 */
class ResumptionDataDB : public ResumptionData {
 public:
  /**
   * @brief Constructor of ResumptionDataDB
   */
  ResumptionDataDB();

  /**
   * @brief allows to destroy ResumptionDataDB object
   */
  virtual ~ResumptionDataDB();

  /**
   * @brief Creates or opens DB and initialize it
   * @return false if DB doesn't initialize
   * otherwise returns true
   */
  virtual bool Init();

  /**
   * @brief Save application persistent info for future resuming to db
   * @param application is application which need to be saved
   */
  virtual void SaveApplication(app_mngr::ApplicationSharedPtr application);
  /**
   * @brief Returns HMI level of application from saved data
   * @param policy_app_id contains policy id of application
   * @param device_id contains id of device on which is running application
   * @return HMI level, if saved data does not contain HMI level method
   * returns -1
   */
  virtual int32_t GetStoredHMILevel(const std::string& policy_app_id,
                                    const std::string& device_id) const;

  /**
   * @brief Checks if saved data of applications have hmi app id
   * @param hmi_app_id - hmi application id
   * @return true if exist, otherwise false
   */
  virtual bool IsHMIApplicationIdExist(uint32_t hmi_app_id) const;
  /**
   * @brief Checks if saved data have application
   * @param policy_app_id - policy application id
   * @param device_id - contains id of device on which is running application
   * @return true if application exists, false otherwise
   */
  virtual bool CheckSavedApplication(const std::string& policy_app_id,
                                     const std::string& device_id);

  /**
   * @brief Retrieves HMI app ID for the given mobile app ID
   * and device ID from stored data.
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return HMI app ID
   */
  virtual uint32_t GetHMIApplicationID(const std::string& policy_app_id,
                                       const std::string& device_id) const;

  /**
   * @brief Increments ignition counter for all registered applications
   * and remember ign_off time stamp
   */
  virtual void OnSuspend();

  /**
   * @brief Retrieves hash ID for the given mobile app ID
   * and device ID from stored information.
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hash_id - parameter which will contain HASH id from saved
   * application
   * @return TRUE if application will be found in saved data otherwise
   * returns FALSE
   */
  virtual bool GetHashId(const std::string& policy_app_id,
                         const std::string& device_id,
                         std::string& hash_id) const;

  /**
   * @brief Decrements ignition counter for all registered applications
   * and remember ign_off time stamp
   */
  virtual void OnAwake();

  /**
   * @brief Retrieves data of saved application for the given mobile app ID
   * and device ID
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param saved_app - parameter which will contain data of saved application
   * @return TRUE if application will be found in saved data otherwise
   * returns FALSE
   */
  virtual bool GetSavedApplication(const std::string& policy_app_id,
                                   const std::string& device_id,
                                   smart_objects::SmartObject& saved_app) const;

  /**
   * @brief Remove application from list of saved applications
   * @param policy_app_id application witch need to be removed
   * @param device_id - contains id of device on which is running application
   * @return return true, if success, otherwise return false
   */
  virtual bool RemoveApplicationFromSaved(const std::string& policy_app_id,
                                          const std::string& device_id);

  /**
   * @brief Get the last ignition off time from LastState
   * @return the last ignition off time from LastState
   */
  virtual uint32_t GetIgnOffTime() const;

  /**
   * @brief Checks if saved data have application
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return 0 if saved data contains application otherwise returns -1
   */
  virtual ssize_t IsApplicationSaved(const std::string& policy_app_id,
                                     const std::string& device_id) const;

  /**
   * @brief Retrieves data from saved application
   * @param  will contain data for resume_ctrl
   */
  virtual void GetDataForLoadResumeData(
      smart_objects::SmartObject& saved_data) const;

  /**
   * @brief Updates HMI level of saved application
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hmi_level - contains hmi level for saved application
   */
  virtual void UpdateHmiLevel(const std::string& policy_app_id,
                              const std::string& device_id,
                              mobile_apis::HMILevel::eType hmi_level);

  /**
   * @brief Re-creates and re-init DB
   * @return true if success, otherwise - false
   */
  bool RefreshDB() const;

  /**
   * @brief Gets all the data from DB to memory
   * @param data Object which holds resumption data
   * @return true if success, otherwise - false
   */
  bool GetAllData(smart_objects::SmartObject& data) const;

  /**
   * @brief Saves all the data to DB
   * @param data Object, which holds resumption data
   * @return true if success, otherwise - false
   */
  bool SaveAllData(const smart_objects::SmartObject& data);

  /**
   * @brief Checks, if DB version is actual to current schema
   * @return true if success, otherwise - false
   */
  bool IsDBVersionActual() const;

  /**
   * @brief Updates DB version accordingly to current schema
   * @return true if success, otherwise - false
   */
  bool UpdateDBVersion() const;

  bool DropAppDataResumption(const std::string& device_id,
                             const std::string& app_id) OVERRIDE;

 private:
  /**
   * @brief Calculates DB version from current schema
   * @return version
   */
  const int32_t GetDBVersion() const;

  /**
   * @brief Retrieves hmi level from db
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hmi_level - will contains hmi level for saved application
   * @return true if application with mobile id and device id has hmi level
   * otherwise returns false
   */
  bool SelectHMILevel(const std::string& policy_app_id,
                      const std::string& device_id,
                      int& hmi_level) const;
  /**
   * @brief Checks existence HMI id in DB
   * @param hmi_app_id - HMI id
   * return true if hmiID is same with saved hmiID otherwise returns false
   */
  bool CheckExistenceHMIId(uint32_t hmi_app_id) const;

  /**
   * @brief Select HMI id from saved application
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hmi_id - will contains hmi id from saved application
   */
  void SelectHMIId(const std::string& policy_app_id,
                   const std::string& device_id,
                   uint32_t& hmi_id) const;

  /**
   * @brief Select hash id from saved application
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hash_id - will contains hash id from saved application
   */
  bool SelectHashId(const std::string& policy_app_id,
                    const std::string& device_id,
                    std::string& hash_id) const;

  /**
   * @brief Select Ign off time
   * @return Ign off time from saved data
   */
  uint32_t SelectIgnOffTime() const;

  /**
   * @brief Checks existence application in DB
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param application_exist will contain true if restored data contain
   * application
   * otherwise will contain false
   * return if we have problem with access to DB returns FALSE otherwise
   * returns TRUE
   */
  bool CheckExistenceApplication(const std::string& policy_app_id,
                                 const std::string& device_id,
                                 bool& application_exist) const;

  /**
   * @brief Retrieves data from saved application
   * @param will contain data for resume_ctrl
   */
  void SelectDataForLoadResumeData(
      smart_objects::SmartObject& saved_data) const;

  /**
   * @brief Deletes saved application from db
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if application data was deleted otherwise returns
   * false
   */
  bool DeleteSavedApplication(const std::string& policy_app_id,
                              const std::string& device_id);

  /**
   * @brief Deletes file from saved application
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if data was deleted otherwise returns
   * false
   */
  bool DeleteSavedFiles(const std::string& policy_app_id,
                        const std::string& device_id);

  /**
   * @brief Deletes submenu from saved application
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if data was deleted otherwise returns
   * false
   */
  bool DeleteSavedSubMenu(const std::string& policy_app_id,
                          const std::string& device_id);

  /**
   * @brief Deletes subscriptions from saved application
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if data was deleted otherwise returns
   * false
   */
  bool DeleteSavedSubscriptions(const std::string& policy_app_id,
                                const std::string& device_id);

  /**
   * @brief Deletes commands from saved application
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if data was deleted otherwise returns
   * false
   */
  bool DeleteSavedCommands(const std::string& policy_app_id,
                           const std::string& device_id);

  /**
   * @brief Deletes choice set from saved application
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if data was deleted otherwise returns
   * false
   */
  bool DeleteSavedChoiceSet(const std::string& policy_app_id,
                            const std::string& device_id);

  /**
   * @brief Deletes global properties from saved application
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if data was deleted otherwise returns
   * false
   */
  bool DeleteSavedGlobalProperties(const std::string& policy_app_id,
                                   const std::string& device_id);
  /**
   * @brief Deletes data from application table
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if data was deleted otherwise returns
   * false
   */
  bool DeleteDataFromApplicationTable(const std::string& policy_app_id,
                                      const std::string& device_id);

  /**
   * @brief Saves files data to DB
   * @param files contains data for saving
   * @param application_primary_key - primary key from DB table application
   * @return true if data was saved successfully otherwise returns
   * false
   */
  bool InsertFilesData(const smart_objects::SmartObject& files,
                       int64_t application_primary_key) const;

  /**
   * @brief Saves submenu data to DB
   * @param submenus contains data for saving
   * @param application_primary_key - primary key from DB table application
   * @return true if data was saved successfully otherwise returns
   * false
   */
  bool InsertSubMenuData(const smart_objects::SmartObject& submenus,
                         int64_t application_primary_key) const;

  /**
   * @brief Saves commands data to DB
   * @param commands contains data for saving
   * @param application_primary_key - primary key from DB table application
   * @return true if data was saved successfully otherwise returns
   * false
   */
  bool InsertCommandsData(const smart_objects::SmartObject& commands,
                          int64_t application_primary_key) const;

  /**
   * @brief Saves subscriptions data to DB
   * @param subscriptions contains data for saving
   * @param application_primary_key - primary key from DB table application
   * @return true if data was saved successfully otherwise returns
   * false
   */
  bool InsertSubscriptionsData(const smart_objects::SmartObject& subscriptions,
                               int64_t application_primary_key) const;

  /**
   * @brief Saves choice set data to DB
   * @param choicesets contains data for saving
   * @param application_primary_key - primary key from DB table application
   * @return true if data was saved successfully otherwise returns
   * false
   */
  bool InsertChoiceSetData(const smart_objects::SmartObject& choicesets,
                           int64_t application_primary_key) const;

  /**
   * @brief Saves globalProperties data to DB
   * @param global_properties contains data for saving
   * @param global_properties_key - will contain primary key from global
   * properties table
   * @return true if data was saved successfully otherwise returns
   * false
   */
  bool InsertGlobalPropertiesData(
      const smart_objects::SmartObject& global_properties,
      int64_t& global_properties_key) const;

  /**
   * @brief Saves application data to DB
   * @param application contains data for saving
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if application data was saved successfully
   * otherwise returns false
   */
  bool SaveApplicationToDB(app_mngr::ApplicationSharedPtr application,
                           const std::string& policy_app_id,
                           const std::string& device_id) const;

  bool SaveApplicationToDB(const smart_objects::SmartObject& application,
                           const std::string& policy_app_id,
                           const std::string& device_id) const;

  /**
   * @brief Updates ignition of count on saved applications after onAwake
   * notification
   */
  void UpdateDataOnAwake();

  /**
   * @brief Execute query for delete data of application
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param text_query - contains text of query
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecQueryToDeleteData(const std::string& policy_app_id,
                             const std::string& device_id,
                             const std::string& text_query);

  /**
   * @brief Execute union query for delete data of application
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param text_query - contains text of query
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecUnionQueryToDeleteData(const std::string& policy_app_id,
                                  const std::string& device_id,
                                  const std::string& text_query);

  /**
   * @brief Execute query in order to insert image to DB
   * @param image_primary_key - will contain primary key from image table
   * @param image contains data for saving to DB
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecInsertImage(int64_t& image_primary_key,
                       const smart_objects::SmartObject& image) const;

  /**
   * @brief Execute query in order to insert choice to DB
   * @param choice_set_key - contain primary key from
   * applicationChoiceSet table
   * @param choice_array contains data for saving to DB
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecInsertChoice(int64_t choice_set_key,
                        const smart_objects::SmartObject& choice_array) const;

  /**
   * @brief Execute query in order to insert vr commands to DB
   * @param primary_key - will contain primary key from command table or
   * choice table
   * @param vr_commands_array contains data for saving to DB
   * @param value indicates which structure contains vrcommands
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecInsertVrCommands(const int64_t primary_key,
                            const smart_objects::SmartObject& vr_commands_array,
                            AccessoryVRCommand value) const;

  /**
   * @brief Execute query in order to insert choice set data to DB
   * @param choice_set_primary_key - will contain primary key from
   * applicationChoiceSet table
   * @param choiceset contains data for saving to DB
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecInsertApplicationChoiceSet(
      int64_t& choice_set_primary_key,
      const smart_objects::SmartObject& choiceset) const;

  /**
   * @brief combines primary key from several table to new table
   * @param first_primary_key - will contain primary key from first DB table
   * @param second_primary_key - will contain primary key from second DB table
   * @param text_query - contains text of query
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecInsertDataToArray(int64_t first_primary_key,
                             int64_t second_primary_key,
                             const std::string& text_query) const;

  /**
   * @brief Execute query in order to insert characters array to DB
   * @param global_properties_key contains primary key from globalproperties
   * table
   * @param characters_array contains data for saving to DB
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecInsertLimitedCharacters(
      int64_t global_properties_key,
      const smart_objects::SmartObject& characters_array) const;

  /**
   * @brief Execute query in order to insert vr help item array to DB
   * @param global_properties_key contains primary key from globalproperties
   * table
   * @param vrhelp_array contains data for saving to DB
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecInsertVRHelpItem(
      int64_t global_properties_key,
      const smart_objects::SmartObject& vrhelp_array) const;

  /**
   * @brief Execute query in order to insert data to ttsChunk table
   * @param tts_chunk contains data for saving to DB
   * @param tts_chunk_key will contain primary key from ttsChunk table
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecInsertTTSChunks(const smart_objects::SmartObject& tts_chunk,
                           int64_t& tts_chunk_key) const;

  /**
   * @brief Execute query in order to insert data to helpTimeoutPromptArray
   * @param global_properties contains data for saving to DB
   * @param global_properties_key contains primary key from globalproperties
   * table
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecInsertHelpTimeoutArray(
      const smart_objects::SmartObject& global_properties,
      int64_t global_properties_key) const;

  /**
   * @brief Execute query in order to insert or update data in application table
   * @param application contains data for saving to DB
   * @param policy_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param application_primary_key will contain primary key from application
   * table
   * @param global_properties_key contains primary key from globalproperties
   * table
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool InsertApplicationData(const ApplicationParams& application,
                             const std::string& policy_app_id,
                             const std::string& device_id,
                             int64_t* application_primary_key,
                             int64_t global_properties_key) const;

  /**
   * @brief Calls InsertApplicationData method
   * @param application contains data for saving to DB
   * @param policy_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @return true if InsertApplicationData works successfully, otherwise
   * returns false;
   */
  bool InsertApplicationData(app_mngr::ApplicationSharedPtr application,
                             const std::string& policy_app_id,
                             const std::string& device_id);

  /**
   * @brief Delete application data where ign_off_count >= application_lifes
   * @param application_lifes contains amount ign off
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool DeleteAppWithIgnCount(const int application_lifes);

  /**
   * @brief Selects data from file table
   * @param policy_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param saved_app will contain data from file table
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectFilesData(const std::string& policy_app_id,
                       const std::string& device_id,
                       smart_objects::SmartObject& saved_app) const;

  /**
   * @brief Selects data from subMenu table
   * @param policy_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param saved_app will contain data from subMenu table
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectSubmenuData(const std::string& policy_app_id,
                         const std::string& device_id,
                         smart_objects::SmartObject& saved_app) const;

  /**
   * @brief Selects data from commands table
   * @param policy_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param saved_app will contain commands
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectCommandData(const std::string& policy_app_id,
                         const std::string& device_id,
                         smart_objects::SmartObject& saved_app) const;

  /**
   * @brief Selects data from applicationSubscribtionsArray table
   * @param policy_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param saved_app will contain subscriptions
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectSubscriptionsData(const std::string& policy_app_id,
                               const std::string& device_id,
                               smart_objects::SmartObject& saved_app) const;

  /**
   * @brief Selects choice set data from DB
   * @param policy_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param saved_app will contain choiceSets
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectChoiceSetData(const std::string& policy_app_id,
                           const std::string& device_id,
                           smart_objects::SmartObject& saved_app) const;

  /**
   * @brief Selects global properties data from DB
   * @param policy_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param saved_app will contain global properties
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectGlobalPropertiesData(const std::string& policy_app_id,
                                  const std::string& device_id,
                                  smart_objects::SmartObject& saved_app) const;

  /**
   * @brief Selects image data from DB
   * @param image_key primary key from image table
   * @param image will contain image data from DB
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectImageData(int64_t image_key,
                       smart_objects::SmartObject& image) const;

  /**
   * @brief Selects ttsChunk data from DB
   * @param tts_chunk_key primary key from ttsChunk table
   * @param tts_chunk will contain ttsChunk data from DB
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectTTSChunkData(int64_t tts_chunk_key,
                          smart_objects::SmartObject& tts_chunk) const;

  /**
   * @brief Selects VR help items data from DB
   * @param global_properties_key primary key from global properties table
   * @param global_properties will contain VR help items
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectVrHelpItemsData(
      int64_t global_properties_key,
      smart_objects::SmartObject& global_properties) const;

  /**
   * @brief Selects table limited character data from DB
   * @param global_properties_key primary key from global properties table
   * @param keyboard_properties will contain table limited character
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectCharactersData(
      int64_t global_properties_key,
      smart_objects::SmartObject& keyboard_properties) const;

  /**
   * @brief Selects data from application table of DB
   * @param policy_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param saved_app will contain data from application table
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectDataFromAppTable(const std::string& policy_app_id,
                              const std::string& device_id,
                              smart_objects::SmartObject& saved_app) const;

  /**
   * @brief Selects count from array table
   * @param count_item will contain amount item in table
   * @param text_query - contains text of query
   * @param policy_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectCountFromArray(uint32_t& count_item,
                            const std::string& text_query,
                            const std::string& policy_app_id,
                            const std::string& device_id) const;

  /**
   * @brief Binds data from smart object with query
   * @param key contains key from smart object
   * @param so contains data which needs to bind with query
   * @param query object of query for DB
   * @param pos contains position for binding query
   */
  void CustomBind(const std::string& key,
                  const smart_objects::SmartObject& so,
                  utils::dbms::SQLQuery& query,
                  const int pos) const;

  /**
   * @brief prepare and bind the same type query
   * @param query object of query for DB
   * @param policy_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param text_query - contains text of query
   * @return true if query was binded successfully otherwise returns
   * false
   */
  bool PrepareSelectQuery(utils::dbms::SQLQuery& query,
                          const std::string& policy_app_id,
                          const std::string& device_id,
                          const std::string& text_query) const;

  /**
   * @brief Updates HMI level and time stamp in application table
   * @param application contains data for saving
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if application data were updated successfully
   * otherwise returns false
   */
  bool UpdateApplicationData(app_mngr::ApplicationConstSharedPtr application,
                             const std::string& policy_app_id,
                             const std::string& device_id);

  /**
   * @brief Writes data to DB after update
   */
  void WriteDb();

  /**
   * @brief returns pointer to data base
   */
  utils::dbms::SQLDatabase* db() const;

  /**
   * @brief Updates grammarID for application
   * @param policy_app_id Application ID
   * @param device_id Device ID
   * @param grammar_id GrammarID
   * @return true if succedeed, otherwise - false
   */
  bool UpdateGrammarID(const std::string& policy_app_id,
                       const std::string& device_id,
                       const uint32_t grammar_id);

  DISALLOW_COPY_AND_ASSIGN(ResumptionDataDB);

  utils::dbms::SQLDatabase* db_;
};
}  // namespace resumption

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_DB_H_
