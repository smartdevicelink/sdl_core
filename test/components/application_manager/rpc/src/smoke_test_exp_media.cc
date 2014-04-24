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

#include "application_manager/commands/command_impl.h"

#include "rpc/smoke_test_exp_media.h"


namespace test {

  void SmokeTestExpMedia(const char* patch) {
    ProtocolHandlerMockup* protocol_handler_test_ =
        ProtocolHandlerMockup::instance();

    HMIMessageHandlerTester* hmi_handler_test =
        HMIMessageHandlerTester::instance();

    am::ApplicationManagerImpl* app_manager_ =
        am::ApplicationManagerImpl::instance();

    utils::SharedPtr<am::Application> appl(app_manager_->application(65537));

    if (!appl.valid()) {
      RegApp();
      sleep(3);
      appl = app_manager_->application(65537);

      if (!appl.valid()) {
        return;
      }
    }

    addTest("smoke_test_exp_media",
            "init_smoke_test_exp_media",
            &InitSmokeTestExpMedia,
            true);

    XmlParser* xmlp = XmlParser::instance();

    xmlp->SetXmlDir(patch);

    if (!xmlp->OpenDir()) {
      printf("\n\n  Opening XML dir failed\n\n");
      return;
    }
    printf("\n\n  Openeding XML dir\n\n");

    while (xmlp->NextXml()) {
      if (xmlp->ReadXml()) {
        printf("\n\n  Opening XML \n\n");
        while (xmlp->MoveToNextTestNode()) {
          utils::SharedPtr<smart::SmartObject> soForParser(
              new smart::SmartObject);
          utils::SharedPtr<smart::SmartObject> soRequest(
              new smart::SmartObject);
          utils::SharedPtr<smart::SmartObject> soResponse(
              new smart::SmartObject);

          if (xmlp->GetRequestFromTest(&(*soForParser))) {
            //application_manager::MessageHelper::PrintSmartObject((*soForParser));
            *soRequest = (*soForParser)["request"];
            *soResponse = (*soForParser)["response"];

            (*soRequest)[jsn::S_PARAMS][am::strings::connection_key] = 65537;
            (*soRequest)[jsn::S_PARAMS][am::strings::message_type] =
                mobile_apis::messageType::request;
            (*soRequest)[jsn::S_PARAMS][am::strings::protocol_version] = 3;
                //am::commands::CommandImpl::protocol_version_;

            addTest<ProtocolHandlerMockup*,
              utils::SharedPtr<smart::SmartObject>,
              utils::SharedPtr<smart::SmartObject>,
              bool>
              ("smoke_test_exp_media",
               (*soForParser)["testName"].asString(),
               protocol_handler_test_,
               soRequest,
               soResponse,
               true);

            if ((*soForParser).keyExists("HMIRequestFirst")) {
              utils::SharedPtr<smart::SmartObject> soHMIRequestFirst(
                  new smart::SmartObject);

              *soHMIRequestFirst = (*soForParser)["HMIRequestFirst"];

              addTest<HMIMessageHandlerTester*,
                utils::SharedPtr<smart::SmartObject>,
                bool>
                ("smoke_test_exp_media",
                 (*soForParser)["testName"].asString() + "HMIFirst",
                 hmi_handler_test,
                 soHMIRequestFirst,
                 true);
            }

            if ((*soForParser).keyExists("HMIRequestSecond")) {
              utils::SharedPtr<smart::SmartObject> soHMIRequestSecond(
                  new smart::SmartObject);

              *soHMIRequestSecond = (*soForParser)["HMIRequestSecond"];

              addTest<HMIMessageHandlerTester*,
                utils::SharedPtr<smart::SmartObject>,
                bool>
                ("smoke_test_exp_media",
                 (*soForParser)["testName"].asString() + "HMISecond",
                 hmi_handler_test, soHMIRequestSecond, true);
            }

            if ((*soForParser).keyExists("HMIRequestThird")) {
              utils::SharedPtr<smart::SmartObject> soHMIRequestSecond(
                  new smart::SmartObject);

              *soHMIRequestSecond = (*soForParser)["HMIRequestThird"];

              addTest<HMIMessageHandlerTester*,
                utils::SharedPtr<smart::SmartObject>,
                bool>
                ("smoke_test_exp_media",
                 (*soForParser)["testName"].asString() + "HMIThird",
                 hmi_handler_test, soHMIRequestSecond, true);
            }

            if ((*soForParser).keyExists("HMIRequestFourth")) {
              utils::SharedPtr<smart::SmartObject> soHMIRequestSecond(
                  new smart::SmartObject);

              *soHMIRequestSecond = (*soForParser)["HMIRequestFourth"];

              addTest<HMIMessageHandlerTester*,
                utils::SharedPtr<smart::SmartObject>,
                bool>
                ("smoke_test_exp_media",
                 (*soForParser)["testName"].asString() + "HMIFourth",
                 hmi_handler_test,
                 soHMIRequestSecond,
                 true);
            }
            sleep(1);
          }
        }
      } else {
        printf("\n\n  Opening XML failed\n\n");
      }
    }

    xmlp->CloseXml();
    xmlp->CloseDir();

    addTest("smoke_test_exp_media",
            "check_not_tested_hmi_request",
            &CheckNotTestedHMIRequest,
            true);
  }  // smoke_test_exp_media

  bool CheckNotTestedHMIRequest() {
    HMIMessageHandlerTester* hmi_handler_test =
        HMIMessageHandlerTester::instance();

    return hmi_handler_test->CheckUntestableRequest();
  }

  bool InitSmokeTestExpMedia() {
    am::ApplicationManagerImpl* app_manager_ =
        am::ApplicationManagerImpl::instance();

    ProtocolHandlerMockup* protocol_handler_test_ =
        ProtocolHandlerMockup::instance();

    HMIMessageHandlerTester* hmi_handler_test =
        HMIMessageHandlerTester::instance();

    utils::SharedPtr<am::Application> appl(app_manager_->application(65537));

    if (!appl.valid()) {
      RegApp();
      sleep(3);
      appl = app_manager_->application(65537);

      if (!appl.valid()) {
        return false;
      }
    }

    ActivateApp();
    sleep(2);
    app_manager_->set_hmi_message_handler(hmi_handler_test);
    app_manager_->set_protocol_handler(protocol_handler_test_);

    return true;
  }
}  // namespace test

