/*
 * Copyright (c) 2014, Ford Motor Company
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

#include "media_manager/audio/from_mic_to_file_recorder_thread.h"
#include <unistd.h>
#include <cstring>
#include <sstream>
#include "interfaces/MOBILE_API.h"
#include "utils/logger.h"

namespace media_manager {

SDL_CREATE_LOG_VARIABLE("MediaManager")

GMainLoop* FromMicToFileRecorderThread::loop = NULL;

// As per spec, AudioPassThru recording is in monaural
static const int kNumAudioChannels = 1;

FromMicToFileRecorderThread::FromMicToFileRecorderThread(
    const std::string& output_file,
    int32_t duration,
    mobile_apis::SamplingRate::eType sampling_rate,
    mobile_apis::BitsPerSample::eType bits_per_sample,
    mobile_apis::AudioType::eType audio_type)
    : threads::ThreadDelegate()
    , argc_(5)
    , argv_(NULL)
    , oKey_("-o")
    , tKey_("-t")
    , sleepThread_(NULL)
    , outputFileName_(output_file)
    , samplingRate_(sampling_rate)
    , bitsPerSample_(bits_per_sample) {
  SDL_LOG_AUTO_TRACE();
  set_record_duration(duration);
  // audio_type is not used as we always employ LPCM
}

FromMicToFileRecorderThread::~FromMicToFileRecorderThread() {
  SDL_LOG_AUTO_TRACE();
  if (sleepThread_) {
    sleepThread_->Stop(threads::Thread::kThreadSoftStop);
    delete sleepThread_->GetDelegate();
    threads::DeleteThread(sleepThread_);
  }
}

void FromMicToFileRecorderThread::set_output_file(
    const std::string& output_file) {
  SDL_LOG_AUTO_TRACE();
  outputFileName_ = output_file;
}

void FromMicToFileRecorderThread::set_record_duration(int32_t duration) {
  SDL_LOG_AUTO_TRACE();

  std::stringstream stringStream;
  stringStream << duration / 1000;
  durationString_ = stringStream.str();
}

void FromMicToFileRecorderThread::initArgs() {
  SDL_LOG_AUTO_TRACE();

  argv_ = new gchar*[argc_];

  argv_[0] = new gchar[14];
  argv_[1] = new gchar[3];
  argv_[2] = new gchar[outputFileName_.length() + 1];
  argv_[3] = new gchar[3];
  argv_[4] = new gchar[durationString_.length() + 1];

  std::strcpy(argv_[0], "AudioManager");
  std::strcpy(argv_[1], oKey_.c_str());
  std::strcpy(argv_[2], outputFileName_.c_str());
  std::strcpy(argv_[3], tKey_.c_str());
  std::strcpy(argv_[4], durationString_.c_str());
}

void FromMicToFileRecorderThread::deinitArgs() {
  SDL_LOG_AUTO_TRACE();

  if (argv_) {
    for (int32_t i = 0; i < argc_; ++i) {
      delete[] argv_[i];
    }
    delete[] argv_;
    argv_ = NULL;
  }
}

void FromMicToFileRecorderThread::threadMain() {
  SDL_LOG_AUTO_TRACE();

  {
    sync_primitives::AutoLock auto_lock(stopFlagLock_);
    shouldBeStoped_ = false;
  }

  initArgs();

  GstElement* pipeline;
  GstElement *alsasrc, *audioconvert, *capsfilter, *wavenc, *filesink;
  GstCaps* audiocaps;
  GstBus* bus;

  const gchar* device = "hw:0,0";
  gchar* outfile = NULL;
  gint duration = -1;
  GOptionContext* context = NULL;
  GError* err = NULL;
  GOptionEntry entries[] = {{"device",
                             'd',
                             0,
                             G_OPTION_ARG_FILENAME,
                             &device,
                             "device file (Default: hw:0,0)",
                             "SRC"},
                            {"output",
                             'o',
                             0,
                             G_OPTION_ARG_FILENAME,
                             &outfile,
                             "save output of the stream to DEST",
                             "DEST"},
                            {"duration",
                             't',
                             0,
                             G_OPTION_ARG_INT,
                             &duration,
                             "length of time in seconds to capture",
                             "int32_t"},
                            {NULL}};
  // g_option_context_parse() modifies params, so keep argc_ and argv_
  int32_t argc = argc_;
  gchar** argv = new gchar*[argc];
  for (int32_t i = 0; i < argc; ++i) {
    argv[i] = argv_[i];
  }

#ifndef GLIB_VERSION_2_32  // g_thread_init() does nothing since 2.32
  if (!g_thread_supported()) {
    g_thread_init(NULL);
  }
#endif
  // Parse the arguments
  context = g_option_context_new("-- M-AUDIO RAW");
  g_option_context_add_main_entries(context, entries, NULL);
  g_option_context_add_group(context, gst_init_get_option_group());
  if (!g_option_context_parse(context, &argc, &argv, &err)) {
    g_error("%s\n", err->message);
  }

  // Check for proper arguments
  if (outfile == NULL) {
    g_error("Must supply destination (-d FILE)\n");
  }

  SDL_LOG_TRACE("Reading from device: " << device);
  SDL_LOG_TRACE("Saving pipeline output to: " << outfile);
  SDL_LOG_TRACE("Duration set to: " << duration);

  // Initialize gstreamer and setup the main loop information
  gst_init(&argc, &argv);

  delete[] argv;
  argv = NULL;

  pipeline = gst_pipeline_new("vga2usb-h264");

  // Set up error handling
  bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
  gst_bus_add_watch(
      bus,
      reinterpret_cast<int32_t (*)(_GstBus*, _GstMessage*, void*)>(recvmsg),
      NULL);
  gst_object_unref(bus);

  // Create all of the elements to be added to the pipeline
  alsasrc = gst_element_factory_make("alsasrc", "alsasrc0");
  audioconvert = gst_element_factory_make("audioconvert", "audioconvert0");
  capsfilter = gst_element_factory_make("capsfilter", "filter0");
  wavenc = gst_element_factory_make("wavenc", "wavenc0");
  filesink = gst_element_factory_make("filesink", "filesink0");

  // Create a capability to specify audio format. It also downmixes the recorded
  // audio to monaural.
  std::string caps_string = create_caps_string();
  SDL_LOG_DEBUG("Using audio caps: " << caps_string);
  audiocaps = gst_caps_from_string(caps_string.c_str());

  // Assert that all the elements were created
  if (!alsasrc || !audioconvert || !capsfilter || !wavenc || !filesink ||
      !audiocaps) {
    g_error("Failed creating one or more of the pipeline elements.\n");
  }

  // Set input and output destinations
  g_object_set(G_OBJECT(alsasrc), "device", device, NULL);
  g_object_set(G_OBJECT(filesink), "location", outfile, NULL);

  // Add the elements to the pipeline
  gst_bin_add_many(GST_BIN(pipeline),
                   alsasrc,
                   audioconvert,
                   capsfilter,
                   wavenc,
                   filesink,
                   NULL);

  // Link the elements
  gst_element_link_many(
      alsasrc, audioconvert, capsfilter, wavenc, filesink, NULL);

  // set the capability
  g_object_set(G_OBJECT(capsfilter), "caps", audiocaps, NULL);
  gst_caps_unref(audiocaps);

  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  SDL_LOG_TRACE("Initializing pipeline ...");
  while (GST_STATE(pipeline) != GST_STATE_PLAYING) {
    bool shouldBeStoped;
    {
      // FIXME(dchmerev@luxoft.com):
      sync_primitives::AutoLock auto_lock(stopFlagLock_);
      shouldBeStoped = shouldBeStoped_;
    }

    if (shouldBeStoped) {
      gst_element_set_state(pipeline, GST_STATE_NULL);
      gst_object_unref(GST_OBJECT(pipeline));
      g_option_context_free(context);

      deinitArgs();
      return;
    }
  }
  SDL_LOG_TRACE("Pipeline started ...\n");

  // Start up a timer for the pipeline
  if (duration > 0) {
    GstTimeout timeout;
    timeout.pipeline = pipeline;
    timeout.duration = duration;

    sleepThread_ =
        threads::CreateThread("SleepThread", new SleepThreadDelegate(timeout));
    sleepThread_->Start();
  }

  loop = g_main_loop_new(NULL, FALSE);

  g_main_loop_run(loop);

  gst_element_set_state(pipeline, GST_STATE_NULL);

  SDL_LOG_TRACE("Deleting pipeline\n");
  gst_object_unref(GST_OBJECT(pipeline));
  g_main_loop_unref(loop);
  g_option_context_free(context);

  deinitArgs();

  loop = NULL;
}

std::string FromMicToFileRecorderThread::create_caps_string() {
  SDL_LOG_AUTO_TRACE();

  std::stringstream ss;
  ss << "audio/x-raw";

  switch (bitsPerSample_) {
    case mobile_apis::BitsPerSample::BitsPerSample_8_BIT:
      // format is 8-bit unsigned
      ss << ",format=(string)U8";
      break;
    case mobile_apis::BitsPerSample::BitsPerSample_16_BIT:
      // format is 16-bit signed, in little endian
      ss << ",format=(string)S16LE";
      break;
    default:
      // do not specify the format; use system default
      break;
  }

  switch (samplingRate_) {
    case mobile_apis::SamplingRate::SamplingRate_8KHZ:
      ss << ",rate=8000";
      break;
    case mobile_apis::SamplingRate::SamplingRate_16KHZ:
      ss << ",rate=16000";
      break;
    case mobile_apis::SamplingRate::SamplingRate_22KHZ:
      ss << ",rate=22050";
      break;
    case mobile_apis::SamplingRate::SamplingRate_44KHZ:
      ss << ",rate=44100";
      break;
    default:
      // do not specify the sampling rate; use system default
      break;
  }

  ss << ",channels=" << kNumAudioChannels;

  return ss.str();
}

FromMicToFileRecorderThread::SleepThreadDelegate::SleepThreadDelegate(
    GstTimeout timeout)
    : threads::ThreadDelegate(), timeout_(timeout) {}

void FromMicToFileRecorderThread::SleepThreadDelegate::threadMain() {
  SDL_LOG_TRACE("Sleep for " << timeout_.duration << " seconds");

  sleep(timeout_.duration);

  if (NULL != loop) {
    if (g_main_loop_is_running(loop)) {
      gst_element_send_event(timeout_.pipeline, gst_event_new_eos());
    }
  }
}

void FromMicToFileRecorderThread::exitThreadMain() {
  SDL_LOG_AUTO_TRACE();

  if (NULL != loop) {
    if (g_main_loop_is_running(loop)) {
      SDL_LOG_TRACE("Quit loop\n");
      g_main_loop_quit(loop);
    }
  }

  if (sleepThread_) {
    SDL_LOG_DEBUG("Stop sleep thread\n");
    sleepThread_->stop();
  }

  SDL_LOG_TRACE("Set should be stopped flag\n");
  sync_primitives::AutoLock auto_lock(stopFlagLock_);
  shouldBeStoped_ = true;
}

}  // namespace media_manager
