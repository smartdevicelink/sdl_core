/**
*
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

#ifndef SRC_COMPONENTS_AUDIO_MANAGER_INCLUDE_AUDIO_MANAGER_AUDIO_MANAGER_IMPL_TEST_H_
#define SRC_COMPONENTS_AUDIO_MANAGER_INCLUDE_AUDIO_MANAGER_AUDIO_MANAGER_IMPL_TEST_H_

#include <pulse/simple.h>
#include <pulse/error.h>
#include "gmock/gmock.h"
#include "audio_manager/audio_manager_impl.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

namespace test {
namespace components {
namespace audio_manager_test {

log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("audio_manager_impl"));

class AudioManagerTest : public ::testing::Test {
 protected:
  virtual void SetUp();
  virtual void TearDown();
};

void AudioManagerTest::SetUp() {
}

void AudioManagerTest::TearDown() {
}

//TEST_F(AudioManagerTest, RecordMicrophoneStream) {
//  audio_manager::AudioManager* audioManager =
//      audio_manager::AudioManagerImpl::getAudioManager();
//
//  audioManager->startMicrophoneRecording(std::string("record.wav"), 5);
//
//  usleep(40000000);
//
//   Sleep for 15 sec
//  usleep(15000000);
//
//  audioManager->stopMicrophoneRecording();
//}

TEST_F(AudioManagerTest, AddAndPlayStream) {
  audio_manager::AudioManager* audioManager =
      audio_manager::AudioManagerImpl::getAudioManager();

  sockaddr device, device_one, device_two;

  device_one.sa_data[0] = 0xf8;
  device_one.sa_data[1] = 0xd0;
  device_one.sa_data[2] = 0xbd;
  device_one.sa_data[3] = 0xac;
  device_one.sa_data[4] = 0xb4;
  device_one.sa_data[5] = 0x5d;

  device_two.sa_data[0] = 0x38;
  device_two.sa_data[1] = 0xe7;
  device_two.sa_data[2] = 0xd8;
  device_two.sa_data[3] = 0x45;
  device_two.sa_data[4] = 0x48;
  device_two.sa_data[5] = 0x27;

  device = device_one;

  audioManager->addA2DPSource(device);
  audioManager->playA2DPSource(device);

  LOG4CXX_TRACE(logger, ".Playing stream");
  while (true) {
    usleep(10000000);
    LOG4CXX_TRACE(logger, ".");
  }

//  usleep(10000000);
//
//  audioManager->removeA2DPSource(device);
//
//  usleep(10000000);
//
//  audioManager->addA2DPSource(device);
//  audioManager->playA2DPSource(device);
//
//  usleep(10000000);
//
//  audioManager->stopA2DPSource(device);
//
//  usleep(10000000);
//
//  audioManager->playA2DPSource(device);
//
//  usleep(10000000);
//
//  audioManager->stopA2DPSource(device);
//
//  usleep(10000000);
//
//  audioManager->removeA2DPSource(device);
}

}  //  namespace audio_manager_test
}  //  namespace components
}  //  namespace test

#endif  // SRC_COMPONENTS_AUDIO_MANAGER_INCLUDE_AUDIO_MANAGER_AUDIO_MANAGER_IMPL_TEST_H_

