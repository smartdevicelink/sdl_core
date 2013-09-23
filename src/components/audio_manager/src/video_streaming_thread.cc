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

#include "audio_manager/video_streaming_thread.h"

namespace audio_manager {

log4cxx::LoggerPtr VideoStreamingThread::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("VideoStreamingThread"));

VideoStreamingThread::VideoStreamingThread()
  : threads::ThreadDelegate(),
    argc_(1) {
  LOG4CXX_TRACE_ENTER(logger_);
  argv_ = new gchar*[argc_];

  argv_[0] = new gchar[14];
  argv_[0] = const_cast<gchar*>(std::string("AudioManager").c_str());
}

void VideoStreamingThread::setVideoFileName(const std::string& fileName) {
  fileName_ = fileName;
}

gboolean VideoStreamingThread::bus_call (GstBus *bus, GstMessage *msg, gpointer data) {
  GMainLoop *loop = (GMainLoop *) data;

  switch (GST_MESSAGE_TYPE (msg)) {

    case GST_MESSAGE_EOS:
      LOG4CXX_TRACE(logger_, "End of stream\n");
      g_main_loop_quit (loop);
      break;

    case GST_MESSAGE_ERROR: {
      gchar  *debug;
      GError *error;

      gst_message_parse_error (msg, &error, &debug);
      g_free (debug);

      LOG4CXX_ERROR(logger_, "Error: " << error->message);
      g_error_free (error);

      g_main_loop_quit (loop);
      break;
    }
    default:
      break;
  }

  return TRUE;
}

void VideoStreamingThread::on_pad_added (GstElement *element, GstPad *pad, gpointer data) {
  GstPad *sinkpad;
  GstElement *decoder = (GstElement *) data;

  /* We can now link this pad with the vorbis-decoder sink pad */
  LOG4CXX_TRACE(logger_, "Dynamic pad created, linking demuxer/decoder");

  sinkpad = gst_element_get_static_pad (decoder, "sink");

  gst_pad_link (pad, sinkpad);

  gst_object_unref (sinkpad);
}

void VideoStreamingThread::threadMain() {
  LOG4CXX_TRACE_ENTER(logger_);

  GstElement *pipeline, *source, *decoder, *encoder, *muxer, *sink;
  GstBus *bus;
  guint bus_watch_id;

  /* Initialisation */
  gst_init (&argc_, &argv_);

  loop = g_main_loop_new (NULL, FALSE);

  /* Create gstreamer elements */
  pipeline = gst_pipeline_new ("audio-player");
  source   = gst_element_factory_make ("filesrc",        "file-source");
  decoder  = gst_element_factory_make ("decodebin",      NULL);
  encoder  = gst_element_factory_make ("theoraenc",      "theoraenc");
  muxer    = gst_element_factory_make ("oggmux",         "oggmux");
  sink     = gst_element_factory_make ("shout2send",     "video-output");

  if (!pipeline || !source || !decoder || !encoder || !muxer || !sink) {
    LOG4CXX_ERROR(logger_, "One element could not be created. Exiting.");
    return /*-1*/;
  }

  /* Set up the pipeline */

  /* we set the input filename to the source element */
  g_object_set (G_OBJECT (source), "location", fileName_.c_str(), NULL);

  g_object_set (G_OBJECT (sink), "password", "hackme", NULL);
  g_object_set (G_OBJECT (sink), "mount", "live", NULL);

  /* we add a message handler */
  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
  gst_object_unref (bus);

  /* we add all elements into the pipeline */
  /* file-source | ogg-demuxer | vorbis-decoder | converter | alsa-output */
  gst_bin_add_many (GST_BIN (pipeline),
                   source, decoder, encoder, muxer, sink, NULL);

  /* we link the elements together */
  /* file-source -> ogg-demuxer ~> vorbis-decoder -> converter -> alsa-output */
  gst_element_link (source, decoder);
  gst_element_link_many (encoder, muxer, sink, NULL);
  g_signal_connect (decoder, "pad-added", G_CALLBACK (on_pad_added), encoder);

  /* note that the demuxer will be linked to the decoder dynamically.
    The reason is that Ogg may contain various streams (for example
    audio and video). The source pad(s) will be created at run time,
    by the demuxer when it detects the amount and nature of streams.
    Therefore we connect a callback function which will be executed
    when the "pad-added" is emitted.*/


  /* Set the pipeline to "playing" state*/
  LOG4CXX_TRACE(logger_, "Now playing");
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  /* Iterate */
  LOG4CXX_TRACE(logger_, "Running...");
  g_main_loop_run (loop);

  /* Out of the main loop, clean up nicely */
  LOG4CXX_TRACE(logger_, "Returned, stopping playback\n");
  gst_element_set_state (pipeline, GST_STATE_NULL);

  LOG4CXX_TRACE(logger_, "Deleting pipeline\n");
  gst_object_unref (GST_OBJECT (pipeline));
  g_source_remove (bus_watch_id);
  g_main_loop_unref (loop);

  return /*0*/;
}

void VideoStreamingThread::exitThreadMain() {
  LOG4CXX_TRACE_ENTER(logger_);

  if(NULL != loop) {
    if(g_main_loop_is_running(loop)) {
      LOG4CXX_TRACE(logger_, "Quit loop\n");
      g_main_loop_quit(loop);
    }
  }
}

}  // namespace audio_manager
