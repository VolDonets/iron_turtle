//
// Created by biba_bo on 2020-08-21.
//

#ifndef IRON_TURTLE_REAR_SIGHT_WEBRTC_MANIPULATION_H
#define IRON_TURTLE_REAR_SIGHT_WEBRTC_MANIPULATION_H

#include "rear_sight_processor/image_processing.h"
#include "rear_sight_processor/rear_sight_processor.h"
//#include "form_detector_ocv/form_detection_processor.h"
#include "form_detector_ocv/form_detection_processor_dnn_caffe.h"

#include <locale.h>
#include <glib.h>
#include <gst/gst.h>
#include <gst/sdp/sdp.h>

#ifdef G_OS_UNIX
#include <glib-unix.h>
#endif

#define GST_USE_UNSTABLE_API
#include <gst/webrtc/webrtc.h>

#include <json-glib/json-glib.h>
#include <string.h>

#include <opencv2/opencv.hpp>
#include <iostream>
#include <seasocks/WebSocket.h>

/*This ia a payload server for possibility to establish client-server connection
 * I tried to use a local payload server, but I had difficulties with configuring it.*/
#define RTP_PAYLOAD_TYPE                "96"
#define STUN_SERVER                     "stun.l.google.com:19302"
#define INTERPOLATION_COEFFICIENT       0.05
//#define STUN_SERVER                     "127.0.0.1:3478"

//Struct which contains pipeline and other GstElements, for comfortable work with connection.
typedef struct _ReceiverEntry ReceiverEntry;

//Creating a structure and destroying it. Last one is used often, when we often change a connection.
ReceiverEntry *create_receiver_entry (seasocks::WebSocket * connection);
void destroy_receiver_entry (gpointer receiver_entry_ptr);

void init_rear_sight_processor();

void on_offer_created_cb (GstPromise * promise, gpointer user_data);
void on_negotiation_needed_cb (GstElement * webrtcbin, gpointer user_data);
void on_ice_candidate_cb (GstElement * webrtcbin, guint mline_index,
                          gchar * candidate, gpointer user_data);

void webrtc_session_handle (const char * message);

void webrtc_pipeline_restart(seasocks::WebSocket *connection);
void webrtc_pipeline_deactivate(seasocks::WebSocket *connection);

int webrtc_gst_loop(seasocks::WebSocket *connection);

static gchar *get_string_from_json_object (JsonObject * object);

void on_zoom_plus_processor();
void on_zoom_minus_processor();

void on_move_left_processor();
void on_move_right_processor();
void on_move_up_processor();
void on_move_down_processor();

struct _ReceiverEntry
{
    seasocks::WebSocket *connection;

    GstElement *ocvvideosrc;
    GstElement *pipeline;
    GstElement *webrtcbin;
};

static std::shared_ptr<FrameParameters> frame_param;
static std::shared_ptr<RearSightProcessor> rear_sight_processor;
static ReceiverEntry *my_receiver_entry;
static GMainLoop *mainloop;
static int count_frames;
//static std::shared_ptr<FormDetectionProcessor> form_detection_processor;
static std::shared_ptr<FormDetectionProcessorDNN> form_detection_processor;
static cv::Rect old_rectangle(0, 0, 0, 0);

#endif //IRON_TURTLE_REAR_SIGHT_WEBRTC_MANIPULATION_H
