/*
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_DB_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_DB_H_
#include "application_manager/resumption/resumption_data.h"
#if __QNX__
#  include "qdb_wrapper/sql_database.h"
#  include "qdb_wrapper/sql_query.h"
#else  // __QNX__
#include "utils/sqlite_wrapper/sql_database.h"
#include "utils/sqlite_wrapper/sql_query.h"
#endif  // __QNX__

namespace resumption {

enum AccessoryVRCommand {
  kVRCommandFromChoice = 0,
  kVRCommandFromCommand
};

class ResumptionDataDB : public ResumptionData {
  public:

  /**
   * @brief Save application persistent info for future resuming to db
   * @param application is application which need to be saved
   */
  virtual void SaveApplication(app_mngr::ApplicationConstSharedPtr application);

  /**
   * @brief Returns HMI level of application from saved data
   * @param m_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @return HMI level if saved data does not contain HMI level method
   * returns -1
   */
  virtual int GetStoredHMILevel(const std::string& m_app_id,
                                const std::string& device_id);

  /**
   * @brief Checks if saved data of applications have hmi app id
   * @param hmi_app_id - hmi application id
   * @return true if exist, otherwise false
   */
  virtual bool IsHMIApplicationIdExist(uint32_t hmi_app_id);

  /**
   * @brief Checks if saved data have application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if application exists, false otherwise
   */
  virtual bool CheckSavedApplication(const std::string& mobile_app_id,
                                     const std::string& device_id);

  /**
   * @brief Retrieves HMI app ID for the given mobile app ID
   * and device ID from stored data.
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return HMI app ID
   */
  virtual uint32_t GetHMIApplicationID(const std::string& mobile_app_id,
                                       const std::string& device_id);

  /**
   * @brief Increments ignition counter for all registered applications
   * and remember ign_off time stamp
   */
  virtual void OnSuspend();

  /**
   * @brief Retrieves hash ID for the given mobile app ID
   * and device ID from stored information.
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hash_id - parameter which will contain HASH id from saved application
   * @return TRUE if application will be found in saved data otherwise
   * returns FALSE
   */
  virtual bool GetHashId(const std::string& mobile_app_id,
                         const std::string& device_id,
                         std::string& hash_id);

  /**
   * @brief Decrements ignition counter for all registered applications
   * and remember ign_off time stamp
   */
  virtual void OnAwake();

  /**
   * @brief Retrieves data of saved appliction for the given mobile app ID
   * and device ID
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param saved_app - parameter which will contain data of saved application
   * @return TRUE if application will be found in saved data otherwise
   * returns FALSE
   */
  virtual bool GetSavedApplication(const std::string& mobile_app_id,
                                   const std::string& device_id,
                                   smart_objects::SmartObject& saved_app);

  /**
   * @brief Remove application from list of saved applications
   * @param mobile_app_id application witch need to be removed
   * @param device_id - contains id of device on which is running application
   * @return return true, if success, otherwise return false
   */
  virtual bool RemoveApplicationFromSaved(const std::string& mobile_app_id,
                                          const std::string& device_id);

  /**
   * @brief Get the last ignition off time from LastState
   * @return the last ignition off time from LastState
   */
  virtual uint32_t GetIgnOffTime();

  /**
   * @brief Checks if saved data have application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return 0 if saved data contains application otherwise returns -1
   */
  virtual int IsApplicationSaved(const std::string& mobile_app_id,
                                 const std::string& device_id);

  /**
   * @brief Retrieves data from saved application
   * @param  will contain data for resume_ctrl
   */
  virtual void GetDataForLoadResumeData(smart_objects::SmartObject& saved_data);

  /**
   * @brief Updates HMI level of saved application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hmi_level - contains hmi level for saved appliction
   */
  virtual void UpdateHmiLevel(const std::string& mobile_app_id,
                              const std::string& device_id,
                              int32_t hmi_level);
  /**
   * @brief Creates or opens DB and initialize it
   * @return false if DB doesn't initialize
   * otherwise returns true
   */
  virtual bool Init();

  virtual ~ResumptionDataDB();
  ResumptionDataDB();



  private:

  /**
   * @brief Retrieves hmi level from db
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hmi_level - will contains hmi level for saved application
   * @return true if application with mobile id and device id has hmi level
   * otherwise returns false
   */
  bool SelectHMILevel(const std::string& m_app_id, const std::string& device_id,
                      int& hmi_level);
  /**
   * @brief Checks existence HMI id in DB
   * @param hmi_app_id - HMI id
   * return true if hmiID is same with saved hmiID otherwise returns false
   */
  bool CheckExistenceHMIId(uint32_t hmi_app_id);

  /**
   * @brief Select HMI id from saved application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hmi_id - will contains hmi id from saved application
   */
  void SelectHMIId(const std::string& mobile_app_id, const std::string& device_id,
                   uint32_t& hmi_id);

  /**
   * @brief Select hash id from saved application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hash_id - will contains hash id from saved application
   */
  bool SelectHashId(const std::string& mobile_app_id,
                    const std::string& device_id,
                    std::string& hash_id);

  /**
   * @brief Select Ign off time
   * @return Ign off time from saved data
   */
  uint32_t SelectIgnOffTime();

  /**
   * @brief Checks existence application in DB
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param application_exist will contain true if restored data contain application
   * otherwise will contain false
   * return if we have problem with access to DB returns FALSE otherwise
   * returns TRUE
   */
  bool CheckExistenceApplication(const std::string& mobile_app_id,
                                 const std::string& device_id,
                                 bool& application_exist);

  /**
   * @brief Retrieves data from saved application
   * @param will contain data for resume_ctrl
   */
  void SelectDataForLoadResumeData(smart_objects::SmartObject& saved_data);



  /**
   * @brief Deletes saved application from db
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if application data was deleted otherwise returns
   * false
   */
  bool DeleteSavedApplication(const std::string& mobile_app_id,
                              const std::string& device_id);

  /**
   * @brief Deletes file from saved application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if data was deleted otherwise returns
   * false
   */
  bool DeleteSavedFiles(const std::string& mobile_app_id,
                        const std::string& device_id);


  /**
   * @brief Deletes submenu from saved application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if data was deleted otherwise returns
   * false
   */
  bool DeleteSavedSubMenu(const std::string& mobile_app_id,
                          const std::string& device_id);

  /**
   * @brief Deletes subscriptions from saved application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if data was deleted otherwise returns
   * false
   */
  bool DeleteSavedSubscriptions(const std::string& mobile_app_id,
                                const std::string& device_id);

  /**
   * @brief Deletes commands from saved application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if data was deleted otherwise returns
   * false
   */
  bool DeleteSavedCommands(const std::string& mobile_app_id,
                           const std::string& device_id);

  /**
   * @brief Deletes choice set from saved application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if data was deleted otherwise returns
   * false
   */
  bool DeleteSavedChoiceSet(const std::string& mobile_app_id,
                            const std::string& device_id);

  /**
   * @brief Deletes global properties from saved application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if data was deleted otherwise returns
   * false
   */
  bool DeleteSavedGlobalProperties(const std::string& mobile_app_id,
                                   const std::string& device_id);

  /**
   * @brief Saves files data to DB
   * @param application contains data for saving
   * @param application_primary_key - primary key from DB table application
   * @return true if data was saved successfully otherwise returns
   * false
   */
  bool InsertFilesData(app_mngr::ApplicationConstSharedPtr application,
                       int64_t application_primary_key);

  /**
   * @brief Saves submenu data to DB
   * @param application contains data for saving
   * @param application_primary_key - primary key from DB table application
   * @return true if data was saved successfully otherwise returns
   * false
   */
  bool InsertSubMenuData(app_mngr::ApplicationConstSharedPtr application,
                         int64_t application_primary_key);

  /**
   * @brief Saves commands data to DB
   * @param application contains data for saving
   * @param application_primary_key - primary key from DB table application
   * @return true if data was saved successfully otherwise returns
   * false
   */
  bool InsertCommandsData(app_mngr::ApplicationConstSharedPtr application,
                          int64_t application_primary_key);

  /**
   * @brief Saves subscriptions data to DB
   * @param application contains data for saving
   * @param application_primary_key - primary key from DB table application
   * @return true if data was saved successfully otherwise returns
   * false
   */
  bool InsertSubscriptionsData(app_mngr::ApplicationConstSharedPtr application,
                               int64_t application_primary_key);

  /**
   * @brief Saves choice set data to DB
   * @param application contains data for saving
   * @param application_primary_key - primary key from DB table application
   * @return true if data was saved successfully otherwise returns
   * false
   */
  bool InsertChoiceSetData(app_mngr::ApplicationConstSharedPtr application,
                           int64_t application_primary_key);

  /**
   * @brief Saves globalProperties data to DB
   * @param application contains data for saving
   * @param global_properties_key - will contain primary key from global properties table
   * @return true if data was saved successfully otherwise returns
   * false
   */
  bool InsertGlobalPropertiesData(app_mngr::ApplicationConstSharedPtr application,
                                  int64_t& global_properties_key);

  /**
   * @brief Saves application data to DB
   * @param application contains data for saving
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param application_exist = true if application exists
   * in DB otherwise = false
   * @return true if application data was saved successfully
   * otherwise returns false
   */
  bool SaveApplicationToDB(app_mngr::ApplicationConstSharedPtr application,
      const std::string& mobile_app_id,
      const std::string& device_id,
      bool application_exist);
  /**
   * @brief Updates ignition of count on saved applications after onAwake
   * notification
   */
  void UpdateDataOnAwake();

  /**
   * @brief Execute query for delete data of application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param text_query - contains text of query
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecQueryToDeleteData(const std::string& mobile_app_id,
                             const std::string& device_id,
                             const std::string& text_query);

  /**
   * @brief Execute union query for delete data of application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param text_query - contains text of query
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecUnionQueryToDeleteData (const std::string& mobile_app_id,
                                   const std::string& device_id,
                                   const std::string& text_query);

  /**
   * @brief Execute query in order to receive application primary key
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param primary_key - will contain primary key from DB
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecSelectPrimaryKeyFromApplication(
      const std::string& mobile_app_id, const std::string& device_id,
      int64_t& primary_key);

  /**
   * @brief Execute query in order to insert image to DB
   * @param image_primary_key - will contain primary key from image table
   * @param image contains data for saving to DB
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecInsertImage(int64_t& image_primary_key,
                       const smart_objects::SmartObject& image);

  /**
   * @brief Execute query in order to insert choice to DB
   * @param choice_set_key - contain primary key from
   * applicationChoiceSet table
   * @param choice_array contains data for saving to DB
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecInsertChoice(int64_t choice_set_key,
                        const smart_objects::SmartObject& choice_array);

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
                            AccessoryVRCommand value);

  /**
   * @brief Execute query in order to insert choice set data to DB
   * @param choice_set_primary_key - will contain primary key from
   * applicationChoiceSet table
   * @param choiceset contains data for saving to DB
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecInsertApplicationChoiceSet(int64_t& choice_set_primary_key,
                                      const smart_objects::SmartObject& choiceset);


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
                             const std::string& text_query);

  /**
   * @brief Execute query in order to insert characters array to DB
   * @param global_properties_key contains primary key from globalproperties table
   * @param characters_array contains data for saving to DB
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecInsertLimitedCharacters(int64_t global_properties_key,
                                   const smart_objects::SmartObject& characters_array);

  /**
   * @brief Execute query in order to insert vr help item array to DB
   * @param global_properties_key contains primary key from globalproperties table
   * @param vrhelp_array contains data for saving to DB
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecInsertVRHelpItem(int64_t global_properties_key,
                            const smart_objects::SmartObject& vrhelp_array);


  /**
   * @brief Execute query in order to insert data to ttsChunk table
   * @param tts_chunk contains data for saving to DB
   * @param tts_chunk_key will contain primary key from ttsChunk table
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecInsertTTSChunks(const smart_objects::SmartObject& tts_chunk,
                           int64_t& tts_chunk_key);

  /**
   * @brief Execute query in order to insert data to helpTimeoutPromptArray
   * @param global_properties contains data for saving to DB
   * @param global_properties_key contains primary key from globalproperties table
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool ExecInsertHelpTimeoutArray(const smart_objects::SmartObject& global_properties,
                                  int64_t global_properties_key);

  /**
   * @brief Execute query in order to insert or update data in application table
   * @param application contains data for saving to DB
   * @param m_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param application_primary_key will contain primary key from application table
   * @param global_properties_key contains primary key from globalproperties table
   * @param application_exist contains TRUE if application exists on DB
   * otherwise contains FALSE.
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool InsertApplicationData(app_mngr::ApplicationConstSharedPtr,
                             const std::string& mobile_app_id,
                             const std::string& device_id,
                             int64_t& application_primary_key,
                             int64_t global_properties_key,
                             bool application_exist);


  /**
   * @brief Delete application data where ign_off_count >= application_lifes
   * @param application_lifes contains amount ign off
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool DeleteAppWithIgnCount(int application_lifes);

  /**
   * @brief Selects data from file table
   * @param mobile_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param saved_app will contain data from file table
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectFilesData(const std::string& mobile_app_id, const std::string& device_id,
                        smart_objects::SmartObject& saved_app);

  /**
   * @brief Selects data from subMenu table
   * @param mobile_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param saved_app will contain data from subMenu table
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectSubmenuData(const std::string& mobile_app_id, const std::string& device_id,
                         smart_objects::SmartObject& saved_app);

  /**
   * @brief Selects data from commands table
   * @param mobile_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param saved_app will contain commands
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectCommandData(const std::string& mobile_app_id, const std::string& device_id,
                         smart_objects::SmartObject& saved_app);

  /**
   * @brief Selects data from applicationSubscribtionsArray table
   * @param mobile_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param saved_app will contain subscriptions
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectSubscriptionsData(const  std::string& mobile_app_id, const std::string& device_id,
                               smart_objects::SmartObject& saved_app);

  /**
   * @brief Selects choice set data from DB
   * @param mobile_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param saved_app will contain choiceSets
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectChoiceSetData(const std::string& mobile_app_id, const std::string& device_id,
                           smart_objects::SmartObject& saved_app);

  /**
   * @brief Selects global properties data from DB
   * @param mobile_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param saved_app will contain global properties
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectGlobalPropertiesData(const std::string& mobile_app_id, const std::string& device_id,
                                  smart_objects::SmartObject& saved_app);

  /**
   * @brief Selects image data from DB
   * @param image_key primary key from image table
   * @param image will contain image data from DB
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectImageData(int64_t image_key, smart_objects::SmartObject& image);

  /**
   * @brief Selects ttsChunk data from DB
   * @param tts_chunk_key primary key from ttsChunk table
   * @param tts_chunk will contain ttsChunk data from DB
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectTTSChunkData(int64_t tts_chunk_key, smart_objects::SmartObject& tts_chunk);

  /**
   * @brief Selects VR help items data from DB
   * @param global_properties_key primary key from global properties table
   * @param global_properties will contain VR help items
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectVrHelpItemsData(int64_t global_properties_key,
                             smart_objects::SmartObject& global_properties);

  /**
   * @brief Selects table limited character data from DB
   * @param global_properties_key primary key from global properties table
   * @param keyboard_properties will contain table limited character
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectCharactersData(int64_t global_properties_key,
                            smart_objects::SmartObject& keyboard_properties);


  /**
   * @brief Selects data from application table of DB
   * @param mobile_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param saved_app will contain data from application table
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectDataFromAppTable(const std::string& mobile_app_id, const std::string& device_id,
                              smart_objects::SmartObject& saved_app);


  /**
   * @brief Selects count from array table
   * @param count_item will contain amount item in table
   * @param text_query - contains text of query
   * @param mobile_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @return true if query was run successfully otherwise returns
   * false
   */
  bool SelectCountFromArray(uint32_t& count_item, const std::string& text_query,
                            const std::string& mobile_app_id,
                            const std::string& device_id);



  /**
   * @brief Binds data from smart object with query
   * @param key contains key from smart object
   * @param so contains data which needs to bind with query
   * @param query object of query for DB
   * @param pos contains position for binding query
   */
  void CustomBind(const std::string& key, const smart_objects::SmartObject& so,
                  utils::dbms::SQLQuery& query, const int pos);

  /**
   * @brief prepare and bind the same type query
   * @param query object of query for DB
   * @param mobile_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @param text_query - contains text of query
   * @return true if query was binded successfully otherwise returns
   * false
   */
  bool PrepareSelectQuery(utils::dbms::SQLQuery& query, const std::string& mobile_app_id,
                           const std::string& device_id, const std::string& text_query);

  /**
   * @brief returns pointer to data base
   */
  utils::dbms::SQLDatabase* db() const;

  DISALLOW_COPY_AND_ASSIGN(ResumptionDataDB);

  utils::dbms::SQLDatabase* db_;

};
}  // namespace resumption

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_DB_H_
