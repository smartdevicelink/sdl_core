/*
 * Copyright (c) 2019, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *Ubuntu 20.04.3 LTS
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

#include "application_manager/interrupt_manager.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "policy/mock_policy_settings.h"
#include "application_manager/mock_rpc_service.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_message_helper.h"


namespace test {
namespace components {
namespace application_manager_test {

using ::test::components::application_manager_test::MockApplicationManager;
using test::components::policy_test::MockPolicyHandlerInterface;
using ::testing::_;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArgPointee;
using namespace application_manager;
using namespace application_manager::interrupt_manager;
using namespace policy_handler_test;
typedef smart_objects::SmartObjectSPtr MessageSharedPtr;

class InterruptManagerTest : public ::testing::Test {
 public:
  typedef NiceMock<application_manager_test::MockRPCService> MockRPCService;

  InterruptManagerTest()
      : interrupt_manager_(new InterruptManager(mock_app_mngr_, mock_policy_settings_)) 
          , kPolicyAppId_("fake_app_id")
          , priority_1("fake_priority")
          , mock_app_ptr_(std::make_shared<NiceMock<MockApplication> >())
          , mock_message_helper_(*application_manager::MockMessageHelper::message_helper_mock()){
         }
  application_manager::MobileMessage CreateMessage() {
      return std::make_shared<application_manager::Message>(
        protocol_handler::MessagePriority::FromServiceType(protocol_handler::ServiceType::kRpc));
  }

 protected:
  void SetUp() OVERRIDE {
    
    DefaultRpcPriority = {};
    std::string rpcPriority[] = {"DialNumber","Alert","PerformAudioPassThru","PerformInteraction","ScrollableMessage","Slider","Speak"};
    DefaultRpcPriority[rpcPriority[0]] = 1;
    DefaultRpcPriority[rpcPriority[1]] = 2;
    DefaultRpcPriority[rpcPriority[2]] = 2;
    DefaultRpcPriority[rpcPriority[3]] = 3;
    DefaultRpcPriority[rpcPriority[4]] = 3;
    DefaultRpcPriority[rpcPriority[5]] = 3;
    DefaultRpcPriority[rpcPriority[6]] = 3;

    DefaultAppPriority = {};
    std::string appPriority[] = {"EMERGENCY","NAVIGATION","VOICE_COMMUNICATION","COMMUNICATION","NORMAL","NONE"};
    DefaultAppPriority[appPriority[0]] = 0;
    DefaultAppPriority[appPriority[1]] = 1;
    DefaultAppPriority[appPriority[2]] = 2;
    DefaultAppPriority[appPriority[3]] = 3;
    DefaultAppPriority[appPriority[4]] = 4;
    DefaultAppPriority[appPriority[5]] = 5;
    
    DefaultHmiStatusPriority = {};
    std::string hmiStatusPriority[] = {"FULL", "LIMITED", "BACKGROUND", "NONE"};
    DefaultHmiStatusPriority[hmiStatusPriority[0]] = 1;
    DefaultHmiStatusPriority[hmiStatusPriority[1]] = 2;
    DefaultHmiStatusPriority[hmiStatusPriority[2]] = 3;
    DefaultHmiStatusPriority[hmiStatusPriority[3]] = 4;
    
    ON_CALL(mock_app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_));
    ON_CALL(mock_policy_handler_, GetRpcPriority())
        .WillByDefault(Return(DefaultRpcPriority));
    ON_CALL(mock_policy_handler_, GetAppPriority())
        .WillByDefault(Return(DefaultAppPriority));
    ON_CALL(mock_policy_handler_, GetHmiStatusPriority())
        .WillByDefault(Return(DefaultHmiStatusPriority)); 

    ON_CALL(mock_app_mngr_, application(_))
        .WillByDefault(Return(mock_app_ptr_)); 
    
    ON_CALL(*mock_app_ptr_, hmi_level(mobile_apis::PredefinedWindows::DEFAULT_WINDOW))
        .WillByDefault(Return(mobile_apis::HMILevel::HMI_NONE));

    ON_CALL(mock_message_helper_, StringifiedFunctionID(mobile_apis::FunctionID::DialNumberID))
      .WillByDefault(Return("DialNumber"));

    ON_CALL(mock_message_helper_, StringifiedFunctionID(mobile_apis::FunctionID::SubtleAlertID))
      .WillByDefault(Return("SubtleAlert"));

    ON_CALL(mock_message_helper_, StringifiedFunctionID(mobile_apis::FunctionID::AlertID))
      .WillByDefault(Return("Alert"));
    
    interrupt_manager_->rpc_priority_table_ = DefaultRpcPriority;
    interrupt_manager_->apptype_priority_table_ = DefaultAppPriority;
    interrupt_manager_->hmists_priority_table_ = DefaultHmiStatusPriority;

    }
  MockRPCService mock_rpc_service_;
  std::unique_ptr<InterruptManager> interrupt_manager_;
  profile::Profile profile_;
  NiceMock<MockPolicySettings> mock_policy_settings_;
  NiceMock<MockApplicationManager> mock_app_mngr_;
  NiceMock<MockPolicyHandlerInterface> mock_policy_handler_;  
  const std::string kPolicyAppId_;
  std::string priority_1;
  std::string priority_2;
  rpc::policy_table_interface_base::rpc_priority_type DefaultRpcPriority;
  rpc::policy_table_interface_base::app_priority_type DefaultAppPriority;
  rpc::policy_table_interface_base::hmi_status_priority_type DefaultHmiStatusPriority;
  std::shared_ptr<NiceMock<MockApplication> > mock_app_ptr_;
  MockMessageHelper& mock_message_helper_;
};

TEST_F(InterruptManagerTest, ReadInterruptSetting_TRUE) {
    EXPECT_CALL(mock_policy_settings_, enable_rpc_conflict_manager())
         .WillRepeatedly(Return(true));
         
    EXPECT_TRUE(interrupt_manager_->ReadInterruptSetting());
}

TEST_F(InterruptManagerTest, ReadInterruptSetting_FALSE) {
    EXPECT_CALL(mock_policy_settings_, enable_rpc_conflict_manager())
         .WillRepeatedly(Return(false));
         
    EXPECT_FALSE(interrupt_manager_->ReadInterruptSetting());
}

TEST_F(InterruptManagerTest, IsSubjectMessageToPrioritized_TRUE) {
    std::shared_ptr<application_manager::Message> message = CreateMessage();
    message->set_function_id(mobile_apis::FunctionID::SubtleAlertID);   
    message->set_message_type(application_manager::kRequest);
    
    EXPECT_TRUE(interrupt_manager_->IsSubjectMessageToPrioritized(message));
}

TEST_F(InterruptManagerTest, CheckRpcInterrupt_NO_INTERRUPT) {

  std::shared_ptr<application_manager::Message> no_interrupt_message = CreateMessage();
  interrupt_manager_->rpc_priority_table_ = DefaultRpcPriority;

  no_interrupt_message->set_function_id(999);
  no_interrupt_message->set_message_type(application_manager::kRequest);  
  
  EXPECT_FALSE(interrupt_manager_->IsSubjectMessageToPrioritized(no_interrupt_message));
  EXPECT_EQ(interrupt_manager_->CheckRpcInterrupt(no_interrupt_message), InterruptCheckResult::NO_INTERRUPT);
  
  no_interrupt_message->set_function_id(mobile_apis::FunctionID::DialNumberID);
  no_interrupt_message->set_message_type(application_manager::kNotification); 

  EXPECT_FALSE(interrupt_manager_->IsSubjectMessageToPrioritized(no_interrupt_message));
  EXPECT_EQ(interrupt_manager_->CheckRpcInterrupt(no_interrupt_message), InterruptCheckResult::NO_INTERRUPT);
  
  no_interrupt_message->set_function_id(mobile_apis::FunctionID::SubtleAlertID);
  no_interrupt_message->set_message_type(application_manager::kNotification); 

  EXPECT_FALSE(interrupt_manager_->IsSubjectMessageToPrioritized(no_interrupt_message));
  EXPECT_EQ(interrupt_manager_->CheckRpcInterrupt(no_interrupt_message), InterruptCheckResult::NO_INTERRUPT);
  
  interrupt_manager_->highest_priority_ons_rpc_ = 0;
  no_interrupt_message->set_function_id(mobile_apis::FunctionID::DialNumberID);
  no_interrupt_message->set_message_type(application_manager::kRequest);

  EXPECT_TRUE(interrupt_manager_->IsSubjectMessageToPrioritized(no_interrupt_message));
  EXPECT_EQ(interrupt_manager_->CheckRpcInterrupt(no_interrupt_message), InterruptCheckResult::NO_INTERRUPT);
  
}

TEST_F(InterruptManagerTest, CheckRpcInterrupt_REJECT_ONS_IN_QUEUE) {
    std::shared_ptr<application_manager::Message> in_queue_message = CreateMessage();
    in_queue_message->set_function_id(mobile_apis::FunctionID::DialNumberID);
    in_queue_message->set_message_type(application_manager::kRequest);              
    interrupt_manager_->highest_priority_ons_rpc_ = in_queue_message; 
    std::shared_ptr<application_manager::Message> reject_ons_in_queue_message = CreateMessage();
    reject_ons_in_queue_message->set_function_id(mobile_apis::FunctionID::DialNumberID);
    reject_ons_in_queue_message->set_message_type(application_manager::kRequest);

    interrupt_manager_->appPriority = "EMERGENCY";
    interrupt_manager_->firstAppPriority = "NONE";
    
    EXPECT_TRUE(interrupt_manager_->IsSubjectMessageToPrioritized(reject_ons_in_queue_message));
    EXPECT_EQ(interrupt_manager_->CheckRpcInterrupt(reject_ons_in_queue_message), InterruptCheckResult::REJECT_ONS_IN_QUEUE);
    
    interrupt_manager_->appPriority = "NONE";

    reject_ons_in_queue_message->set_function_id(mobile_apis::FunctionID::DialNumberID);
    in_queue_message->set_function_id(mobile_apis::FunctionID::AlertID);
    
    EXPECT_EQ(interrupt_manager_->CheckRpcInterrupt(reject_ons_in_queue_message), InterruptCheckResult::REJECT_ONS_IN_QUEUE);
    in_queue_message->set_function_id(mobile_apis::FunctionID::DialNumberID);

    interrupt_manager_->appPriority = "NAVIGATION";
    interrupt_manager_->firstAppPriority = "NONE";
    
    EXPECT_EQ(interrupt_manager_->CheckRpcInterrupt(reject_ons_in_queue_message), InterruptCheckResult::REJECT_ONS_IN_QUEUE);
    
    interrupt_manager_->appPriority = "NONE";
    
    EXPECT_EQ(interrupt_manager_->CheckRpcInterrupt(reject_ons_in_queue_message), InterruptCheckResult::REJECT_ONS_IN_QUEUE);
}

TEST_F(InterruptManagerTest, CheckRpcInterrupt_REJECT_RECEIVED_ONS) {
    std::shared_ptr<application_manager::Message> reject_received_ons_message = CreateMessage();

    reject_received_ons_message->set_function_id(mobile_apis::FunctionID::DialNumberID);
    reject_received_ons_message->set_message_type(application_manager::kRequest);

    std::shared_ptr<application_manager::Message> in_queue_message = CreateMessage();

    in_queue_message->set_function_id(mobile_apis::FunctionID::DialNumberID);
    in_queue_message->set_message_type(application_manager::kRequest);

    interrupt_manager_->appPriority = "NONE";
    interrupt_manager_->firstAppPriority = "EMERGENCY";

    interrupt_manager_->highest_priority_ons_rpc_ = in_queue_message;
    
    EXPECT_EQ(interrupt_manager_->CheckRpcInterrupt(reject_received_ons_message), InterruptCheckResult::REJECT_RECEIVED_ONS);
    
    interrupt_manager_->firstAppPriority = "NONE";
    reject_received_ons_message->set_function_id(mobile_apis::FunctionID::AlertID);
    
    EXPECT_EQ(interrupt_manager_->CheckRpcInterrupt(reject_received_ons_message), InterruptCheckResult::REJECT_RECEIVED_ONS);  

    reject_received_ons_message->set_function_id(mobile_apis::FunctionID::DialNumberID);   
    interrupt_manager_->firstAppPriority = "NAVIGATION";
    
    EXPECT_EQ(interrupt_manager_->CheckRpcInterrupt(reject_received_ons_message), InterruptCheckResult::REJECT_RECEIVED_ONS);
    
    interrupt_manager_->app_hmi_level = "NONE";
    interrupt_manager_->first_app_hmi_level = "FULL";
    
    EXPECT_EQ(interrupt_manager_->CheckRpcInterrupt(reject_received_ons_message), InterruptCheckResult::REJECT_RECEIVED_ONS);  
}

TEST_F(InterruptManagerTest, DoRpcReject_SUCCESS) {
  mobile_apis::FunctionID::eType function_id = mobile_apis::FunctionID::eType::DialNumberID;

  std::list<int32_t> test_reject_rpc_list_{function_id};
  interrupt_manager_->reject_rpc_list_ = test_reject_rpc_list_;

  std::shared_ptr<application_manager::Message> message = CreateMessage();
  interrupt_manager_->highest_priority_ons_rpc_ = message;

  EXPECT_TRUE(interrupt_manager_->DoRpcReject(mobile_apis::FunctionID::eType::DialNumberID));
}

TEST_F(InterruptManagerTest, DoRpcReject_UNSUCCESS) {
  mobile_apis::FunctionID::eType function_id = mobile_apis::FunctionID::eType::DialNumberID;
  std::list<int32_t> test_reject_rpc_list_{function_id};
  std::shared_ptr<application_manager::Message> message = CreateMessage();
  interrupt_manager_->highest_priority_ons_rpc_ = message;

  interrupt_manager_->reject_rpc_list_.push_front(message->function_id());
  
  EXPECT_FALSE(interrupt_manager_->DoRpcReject(mobile_apis::FunctionID::eType::AlertID));
}

}  // namespace application_manager_test
}  // namespace components
}  // namespace test

