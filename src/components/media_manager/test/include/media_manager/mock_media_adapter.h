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

#ifndef SRC_COMPONENTS_MEDIA_MANAGER_TEST_INCLUDE_MEDIA_MANAGER_MOCK_MEDIA_ADAPTER_H_
#define SRC_COMPONENTS_MEDIA_MANAGER_TEST_INCLUDE_MEDIA_MANAGER_MOCK_MEDIA_ADAPTER_H_

#include "gmock/gmock.h"
#include "media_manager/media_manager.h"
#include "media_manager/media_adapter.h"

namespace test {
namespace components {
namespace media_manager_test {

/*
 * MOCK implementation of ::media_manager::MediaAdapter
 */
class MockMediaAdapter : public ::media_manager::MediaAdapter {
 public:
  MOCK_METHOD2(SendData,
               void(int32_t application_key,
                    const ::protocol_handler::RawMessagePtr message));
  MOCK_METHOD1(StartActivity, void(int32_t application_key));
  MOCK_METHOD1(StopActivity, void(int32_t application_key));
  MOCK_CONST_METHOD1(is_app_performing_activity, bool(int32_t application_key));
};

}  // namespace media_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_MEDIA_MANAGER_TEST_INCLUDE_MEDIA_MANAGER_MOCK_MEDIA_ADAPTER_H_
