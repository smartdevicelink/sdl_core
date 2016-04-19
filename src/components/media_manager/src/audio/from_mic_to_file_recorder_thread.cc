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
#include "media_manager/audio/from_mic_to_file_recorder_thread.h"
#include <gst/gst.h>

#if defined(OS_POSIX)
#include <unistd.h>
#include "utils/timer_task_impl.h"
#elif defined(OS_WINDOWS)
#include "utils/threads/thread.h"
#endif

#include <sstream>
#include "utils/logger.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "FromMicToFileRecorderThread")

namespace media_manager {

gboolean HandleBusMessage(GstBus* bus, GstMessage* message, gpointer data) {
  GMainLoop* loop = static_cast<GMainLoop*>(data);
  switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_EOS: {
      LOGGER_INFO(logger_, "End of stream");

      g_main_loop_quit(loop);
      break;
    }
    case GST_MESSAGE_ERROR: {
      gchar* debug;
      GError* error;

      gst_message_parse_error(message, &error, &debug);
      g_free(debug);

      LOGGER_ERROR(logger_, error->message);
      g_error_free(error);

      g_main_loop_quit(loop);
      break;
    }
    default:
      break;
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////
/// media_manager::FromMicToFileRecorderThread::Impl
////////////////////////////////////////////////////////////////////////////////

class FromMicToFileRecorderThread::Impl {
 public:
  Impl(const std::string oKey_,
       const std::string tKey_,
       const std::string outputFileName,
       int32_t duration);
  ~Impl();

  void startGstLoop();
  void stopGstLoop();

  int32_t getDuration() const;
  void setDuration(const int32_t duration);
  void setShouldBeStoped(const bool shouldBeStoped);

 private:
  int32_t argc_;
  gchar** argv_;
  GstElement* pipeline_;
  sync_primitives::atomic_int32 duration_;

  bool shouldBeStoped_;
  sync_primitives::Lock stopFlagLock_;

  static GMainLoop* loop;
  DISALLOW_COPY_AND_ASSIGN(Impl);
};
}  // namespace media_manager

GMainLoop* media_manager::FromMicToFileRecorderThread::Impl::loop = NULL;

media_manager::FromMicToFileRecorderThread::Impl::Impl(
    const std::string oKey_,
    const std::string tKey_,
    const std::string outputFileName,
    int32_t duration)
  : duration_(duration) {
  argc_ = 5;
  argv_ = new gchar* [argc_];

  std::stringstream stringStream;
  stringStream << duration / 1000;
  std::string durationString = stringStream.str();

  argv_[0] = new gchar[14];
  argv_[1] = new gchar[3];
  argv_[2] = new gchar[outputFileName.length() + 1];
  argv_[3] = new gchar[3];
  argv_[4] = new gchar[durationString.length() + 1];

  argv_[0] = const_cast<gchar*>(std::string("AudioManager").c_str());
  argv_[1] = const_cast<gchar*>(oKey_.c_str());
  argv_[2] = const_cast<gchar*>(outputFileName.c_str());
  argv_[3] = const_cast<gchar*>(tKey_.c_str());
  argv_[4] = const_cast<gchar*>(durationString.c_str());
}

media_manager::FromMicToFileRecorderThread::Impl::~Impl() {}

void media_manager::FromMicToFileRecorderThread::Impl::setDuration(
    const int32_t duration) {
  duration_ = duration;
}

int32_t media_manager::FromMicToFileRecorderThread::Impl::getDuration() const {
  return duration_;
}

void media_manager::FromMicToFileRecorderThread::Impl::setShouldBeStoped(
    const bool shouldBeStoped) {
  sync_primitives::AutoLock auto_lock(stopFlagLock_);
  shouldBeStoped_ = shouldBeStoped;
}

void media_manager::FromMicToFileRecorderThread::Impl::stopGstLoop() {
  if (loop) {
    if (g_main_loop_is_running(loop)) {
      gst_element_send_event(pipeline_, gst_event_new_eos());
    }
  }
}

void media_manager::FromMicToFileRecorderThread::Impl::startGstLoop() {
  LOGGER_AUTO_TRACE(logger_);
  GstElement* alsasrc, *wavenc, *filesink;
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
#ifndef GLIB_VERSION_2_32  // g_thread_init() does nothing since 2.32
  if (!g_thread_supported()) {
    g_thread_init(NULL);
  }
#endif
  duration_ = static_cast<int32_t>(duration);
  // Parse the arguments
  context = g_option_context_new("-- M-AUDIO RAW");
  g_option_context_add_main_entries(context, entries, NULL);
  g_option_context_add_group(context, gst_init_get_option_group());
  if (!g_option_context_parse(context, &argc_, &argv_, &err)) {
    g_error("%s\n", err->message);
  }

  // Check for proper arguments
  if (outfile == NULL) {
    g_error("Must supply destination (-d FILE)\n");
  }

  LOGGER_TRACE(logger_, "Reading from device: " << device);
  LOGGER_TRACE(logger_, "Saving pipeline output to: " << outfile);
  LOGGER_TRACE(logger_, "Duration set to: " << duration);

  // Initialize gstreamer and setup the main loop information
  gst_init(&argc_, &argv_);

  pipeline_ = gst_pipeline_new("vga2usb-h264");

  // Set up error handling
  bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline_));
  gst_bus_add_watch(bus, HandleBusMessage, NULL);
  gst_object_unref(bus);

  // Create all of the elements to be added to the pipeline
  alsasrc = gst_element_factory_make("alsasrc", "alsasrc0");
  wavenc = gst_element_factory_make("wavenc", "wavenc0");
  filesink = gst_element_factory_make("filesink", "filesink0");

  // Assert that all the elements were created
  if (!alsasrc || !wavenc || !filesink) {
    g_error("Failed creating one or more of the pipeline elements.\n");
  }

  // Set input and output destinations
  g_object_set(G_OBJECT(alsasrc), "device", device, NULL);
  g_object_set(G_OBJECT(filesink), "location", outfile, NULL);

  // Add the elements to the pipeline
  gst_bin_add_many(GST_BIN(pipeline_), alsasrc, wavenc, filesink, NULL);

  // Link the elements
  gst_element_link_many(alsasrc, wavenc, filesink, NULL);

  gst_element_set_state(pipeline_, GST_STATE_PLAYING);

  LOGGER_TRACE(logger_, "Initializing pipeline ...");
  while (GST_STATE(pipeline_) != GST_STATE_PLAYING) {
    bool shouldBeStoped;
    {
      // FIXME(dchmerev@luxoft.com):
      sync_primitives::AutoLock auto_lock(stopFlagLock_);
      shouldBeStoped = shouldBeStoped_;
    }

    if (shouldBeStoped) {
      gst_element_set_state(pipeline_, GST_STATE_NULL);
      gst_object_unref(GST_OBJECT(pipeline_));
      g_option_context_free(context);

      if (argv_) {
        delete[] argv_;
        argv_ = NULL;
      }
      return;
    }
  }
  LOGGER_TRACE(logger_, "Pipeline started ...\n");

  loop = g_main_loop_new(NULL, FALSE);

  g_main_loop_run(loop);

  gst_element_set_state(pipeline_, GST_STATE_NULL);

  LOGGER_TRACE(logger_, "Deleting pipeline\n");
  gst_object_unref(GST_OBJECT(pipeline_));
  g_main_loop_unref(loop);
  g_option_context_free(context);

  if (argv_) {
    delete[] argv_;
    argv_ = NULL;
  }
  loop = NULL;
}

////////////////////////////////////////////////////////////////////////////////
/// media_manager::FromMicToFileRecorderThread::Impl
////////////////////////////////////////////////////////////////////////////////

media_manager::FromMicToFileRecorderThread::FromMicToFileRecorderThread(
    const std::string& output_file, int32_t duration)
    : threads::ThreadDelegate()
    , impl_(new Impl("-o", "-t", output_file, duration)) {
  LOGGER_AUTO_TRACE(logger_);
  sleep_thread_ =
      new timer::Timer(
        "AudioFromMicSuspend",
        new timer::TimerTaskImpl<FromMicToFileRecorderThread>(
          this,
          &FromMicToFileRecorderThread::onFromMicToFileRecorderThreadSuspned));
}

media_manager::FromMicToFileRecorderThread::~FromMicToFileRecorderThread() {
  LOGGER_AUTO_TRACE(logger_);
  if (sleep_thread_) {
    sleep_thread_->Stop();
  }
  delete impl_;
  impl_ = NULL;
}

void media_manager::FromMicToFileRecorderThread::setRecordDuration(
    int32_t duration) {
  LOGGER_AUTO_TRACE(logger_);
  impl_->setDuration(duration);
}

void media_manager::FromMicToFileRecorderThread::threadMain() {
  LOGGER_AUTO_TRACE(logger_);

  impl_->setShouldBeStoped(false);
  impl_->startGstLoop();

  // Start up a timer for the pipeline
  if (impl_->getDuration() > 0) {
    sleep_thread_->Start(impl_->getDuration(), true);
  }
}

void media_manager::FromMicToFileRecorderThread::
    onFromMicToFileRecorderThreadSuspned() {
  LOGGER_AUTO_TRACE(logger_);
  impl_->stopGstLoop();
  LOGGER_TRACE(logger_, "Set should be stopped flag\n");
  impl_->setShouldBeStoped(true);
}

void media_manager::FromMicToFileRecorderThread::exitThreadMain() {
  LOGGER_AUTO_TRACE(logger_);

  impl_->stopGstLoop();
  if (sleep_thread_) {
    LOGGER_DEBUG(logger_, "Stop sleep thread\n");
    sleep_thread_->Stop();
  }

  LOGGER_TRACE(logger_, "Set should be stopped flag\n");
  impl_->setShouldBeStoped(true);
}
