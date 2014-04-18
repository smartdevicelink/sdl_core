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

#include "rpc/init_start_data.h"
#include "life_cycle.h"

void InitStartData() {
  printf("\n\n\n in initStartData\n\n\n");

  CREATE_LOGGERPTR_LOCAL(logger_, "initStartData")

  LOG4CXX_INFO(logger_, " Application started!");

  // --------------------------------------------------------------------------
  // Components initialization

  if (!main_namespace::LifeCycle::instance()->StartComponents()) {
    LOG4CXX_INFO(logger_, "Start Components fail");
    exit(EXIT_FAILURE);
  }

  // Third-Party components initialization.

  if (!main_namespace::LifeCycle::instance()->InitMessageSystem()) {
    main_namespace::LifeCycle::instance()->StopComponents();
  // without this line log4cxx threads continue using some
  // instances destroyed by exit()
    log4cxx::Logger::getRootLogger()->closeNestedAppenders();
    exit(EXIT_FAILURE);
  }
  LOG4CXX_INFO(logger_, "InitMessageBroker successful");

  if (profile::Profile::instance()->launch_hmi()) {
    if (profile::Profile::instance()->server_address() ==
        std::string(InitializeHMI::kLocalHostAddress)) {
      LOG4CXX_INFO(logger_, "Start HMI on localhost");

      sleep(1);
      /*
      threads::ThreadOptions threadOption(16384);
      InitializeHMI::InitHMI* initHMI = new InitializeHMI::InitHMI;
      threads::Thread threadInitializeHMI("InitializeHMI", initHMI);
      //initHMI->threadMain();
      //threadInitializeHMI.startWithOptions(threadOption);
      threadInitializeHMI.start();
      threadInitializeHMI.join();
      */

      if (!InitializeHMI::InitHmi()) {
        main_namespace::LifeCycle::instance()->StopComponents();
        // without this line log4cxx threads continue using some
        // instances destroyed by exit()
        log4cxx::Logger::getRootLogger()->closeNestedAppenders();
        exit(EXIT_FAILURE);
      }
      LOG4CXX_INFO(logger_, "InitHmi successful");
    }
  }
  utils::SubscribeToTerminateSignal(
    &main_namespace::LifeCycle::StopComponentsOnSignal);
}


void RegApp() {
  connection_handler::ConnectionHandlerImpl* connection_handler_ =
      connection_handler::ConnectionHandlerImpl::instance();

  application_manager::ApplicationManagerImpl* app_manager_ =
      application_manager::ApplicationManagerImpl::instance();

  utils::SharedPtr<am::Application> appl(app_manager_->application(65537));

  if (appl.valid()) {
    return;
  }

  printf("\n\n\n Registration Application \n\n\n");

  transport_manager::DeviceHandle device_handler_test = 1;
  std::string mac_address_test = "255.255.255.0";
  std::string name_test = "test_DeviceInfo";

  transport_manager::ConnectionUID connection_id_test = 1;

  transport_manager::DeviceInfo device_info_test(
      device_handler_test,
      mac_address_test,
      name_test);

  // std::vector<transport_manager::DeviceInfo> device_list_test;
  // device_list_test.push_back(device_info_test);
  // connection_handler_->OnDeviceListUpdated(device_list_test);
  connection_handler_->OnDeviceAdded(device_info_test);

  connection_handler_->OnConnectionEstablished(
      device_info_test,
      connection_id_test);
  sleep(5);

  utils::SharedPtr<smart::SmartObject> AppRegRequest(new smart::SmartObject);

  // -------------------------------------------------------------------------
  (*AppRegRequest)[jsn::S_PARAMS][am::strings::function_id] =
      mobile_apis::FunctionID::RegisterAppInterfaceID;
  (*AppRegRequest)[jsn::S_PARAMS][am::strings::message_type] =
      mobile_apis::messageType::request;
  (*AppRegRequest)[jsn::S_PARAMS][am::strings::protocol_version] = 2;
  (*AppRegRequest)[jsn::S_PARAMS][am::strings::connection_key] = 65537;

  (*AppRegRequest)[jsn::S_MSG_PARAMS][am::strings::app_name] =
      "SyncProxyTester";
  (*AppRegRequest)[jsn::S_MSG_PARAMS][am::strings::available] = true;
  (*AppRegRequest)[jsn::S_MSG_PARAMS][am::strings::language_desired] =
      hmi_apis::Common_Language::EN_US;
  (*AppRegRequest)[jsn::S_MSG_PARAMS][am::strings::hmi_display_language_desired]
                                      = hmi_apis::Common_Language::EN_US;

  (*AppRegRequest)[jsn::S_MSG_PARAMS][am::strings::app_id] = "65537";
  (*AppRegRequest)[jsn::S_MSG_PARAMS]
                   [am::strings::sync_msg_version]["majorVersion"] = 2;
  (*AppRegRequest)[jsn::S_MSG_PARAMS]
                   [am::strings::sync_msg_version]["minorVersion"] = 2;
  (*AppRegRequest)[jsn::S_MSG_PARAMS]
                   [am::strings::is_media_application] = true;
  (*AppRegRequest)[jsn::S_MSG_PARAMS]
                   [am::strings::app_hmi_type][0] = "NAVIGATION";
  (*AppRegRequest)[jsn::S_MSG_PARAMS]
                   [am::strings::ngn_media_screen_app_name] = "SyncP";
  (*AppRegRequest)[jsn::S_MSG_PARAMS]
                   [am::strings::vr_synonyms][0] = "VR SyncProxyTester";

  app_manager_->ManageMobileCommand(AppRegRequest);

  // -------------------------------------------------------------------------
  // Activate App
  sleep(2);
  utils::SharedPtr<smart::SmartObject> ActivateApp(new smart::SmartObject);
  (*ActivateApp)[jsn::S_PARAMS][am::strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_OnAppActivated;
  (*ActivateApp)[jsn::S_PARAMS][am::strings::connection_key] = 65537;
  (*ActivateApp)[jsn::S_PARAMS][am::strings::message_type] =
      mobile_apis::messageType::request;
  (*ActivateApp)[jsn::S_MSG_PARAMS][am::strings::app_id] = "65537";

  app_manager_->ManageHMICommand(ActivateApp);
}
