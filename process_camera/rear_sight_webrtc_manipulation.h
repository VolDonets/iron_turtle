//
// Created by biba_bo on 2020-08-21.
//

#ifndef IRON_TURTLE_REAR_SIGHT_WEBRTC_MANIPULATION_H
#define IRON_TURTLE_REAR_SIGHT_WEBRTC_MANIPULATION_H

#include "rear_sight_processor/image_processing.h"
#include "rear_sight_processor/rear_sight_processor.h"
#include "form_detector_ocv/form_detection_processor.h"

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
#define INTERPOLATION_COEFFICIENT       0.2
//#define STUN_SERVER                     "127.0.0.1:3478"

/// Struct which contains pipeline and other GstElements, for comfortable work with connection.
typedef struct _ReceiverEntry ReceiverEntry;

#ifdef MY_PURSUIT_TESTING
/// this constant contains a basic X coord for the start pursuit rectangle
constexpr int START_PURSUIT_X = 240;
/// this constant contains a basic Y coord for the start pursuit rectangle
constexpr int START_PURSUIT_Y = 180;
/// this constant contains a basic width for the start pursuit rectangle
constexpr int START_PURSUIT_WIDTH = 160;
/// this constant contains a basic height for the start pursuit rectangle
constexpr int START_PURSUIT_HEIGHT = 120;
/// this is a step for the X axis for the pursuit rectangle moving
constexpr int PURSUIT_STEP_X = 5;
/// this is a step for the changing a pursuit area
constexpr double PURSUIT_SCALE_STEP = 0.02;

/// this variable contains scale for the pursuit rectangle
static double scalePursuit = 0.25;
/// this variable contains current position by 'x' the pursuit rectangle
static int xPursuitRect = START_PURSUIT_X;
/// this variable contains current position by 'y' the pursuit rectangle
static int yPursuitRect = START_PURSUIT_Y;
/// this variable contains current width of the pursuit rectangle
static int widthPursuitRect = START_PURSUIT_WIDTH;
/// this variable contains current height of the pursuit rectangle
static int heightPursuitRect = START_PURSUIT_HEIGHT;

/// this function increase a pursuit current area
void increase_object_rectangle();

/// this function decrease a pursuit current area
void decrease_object_rectangle();

/// this function moves lefter an interested object
void move_lefter_object_rectangle();

/// this fucntion moves righter an interested object
void move_righter_object_rectangle();
#endif //MY_PURSUIT_TESTING


/// Creating a ReceiverEntry structure. Usually it uses when happens a new connection.
ReceiverEntry *create_receiver_entry(seasocks::WebSocket *connection);

/// Destructing a ReceiverEntry structure. Last one is used often, when we often change a connection.
void destroy_receiver_entry(gpointer receiver_entry_ptr);

/** This .h file created in the C++ class style, but, cause some functions are called in other places, I cannot use a class structure,
 * so this one just initialize variables for processing a frame, and an object recognition object (here is also the thread for this processing).*/
void init_rear_sight_processor();

/** This function is one of the three steps of the WebRTC connection processing.
 * This one are called when the offer created and the realtime stream can be started.*/
void on_offer_created_cb(GstPromise *promise, gpointer user_data);

/** This function is one of the three steps of the WebRTC connection processing.
 * This one are called when the GST pipeline are prepared.*/
void on_negotiation_needed_cb(GstElement *webrtcbin, gpointer user_data);

/** This function is one of the three steps of the WebRTC connection processing.
 * This one are called when new user want to get a real time media stream.*/
void on_ice_candidate_cb(GstElement *webrtcbin, guint mline_index,
                         gchar *candidate, gpointer user_data);

/** This function handle webrtc session, and dependently from the client message it calls functions:
 * 'on_offer_created_cb', 'on_negotiation_needed_cb', 'on_ice_candidate_cb', or throw an exception.*/
void webrtc_session_handle(const char *message);

/** This function create a new ReceiverEntry */
void webrtc_pipeline_restart(seasocks::WebSocket *connection);

/** This function doing disconnection of the connected client, and breaks an pipeline*/
void webrtc_pipeline_deactivate(seasocks::WebSocket *connection);

/** This function is a realization of GST loop (kind of the independent thread) for processing a current pipeline*/
int webrtc_gst_loop(seasocks::WebSocket *connection);

/** This function is converts a JsonObjet to the string*/
static gchar *get_string_from_json_object(JsonObject *object);

/// This function is manage a cropping process, and this one DECREASE a cropped image size
void on_zoom_plus_processor();

/// This function is manage a cropping process, and this one INCREASE a cropped image size
void on_zoom_minus_processor();

/// This function is manage a cropping process, and this one MOVE a cropped window to the LEFT side
void on_move_left_processor();

/// This function is manage a cropping process, and this one MOVE a cropped window to the RIGHT side
void on_move_right_processor();

/// This function is manage a cropping process, and this one MOVE a cropped window to the TOP side
void on_move_up_processor();

/// This function is manage a cropping process, and this one MOVE a cropped window to the BOTTOM side
void on_move_down_processor();

/// This function is sets a speed values for printing into pipeline
void set_speed_values_gst_pipeline_info(double currentLeftSpeed, double currentRightSpeed);

/** This is a realization of the ReceiverEntry and this one contains
 * seasocks::WebSocket connection - this is a current connection and it used for establishing WebRTC connection
 * GstElement *ocvvideosrc - this is a Gst element, used for modification a pipeline
 * GstElement *pipeline - this is a Gst element, used for controlling whole pipeline
 * GstElement *webrtcbin - this is a Gst element, used for establishing WebRTC connection*/
struct _ReceiverEntry {
    seasocks::WebSocket *connection;

    GstElement *ocvvideosrc;
    GstElement *pipeline;
    GstElement *webrtcbin;
};

/// This is a smart pointer to the FrameParameters - this is used for the cropping window.
static std::shared_ptr<FrameParameters> frame_param;
/// This is a smart pointer to the RearSightProcessor - this is used for modification a currant frame and calculating bounding box
static std::shared_ptr<RearSightProcessor> rear_sight_processor;
/// This is a pointer to ReceiverEntry - used for establishing WebRTC connection and modification a stream in the pipeline
static ReceiverEntry *my_receiver_entry;
/// This is a pointer to GMainLoop - this is a kind of thread for the GST pipeline
static GMainLoop *mainloop;
/// This is frame counter - this one is says us how many frames we should skip, before send a frame to the FormDetectionProcessor
static int count_frames;
/// This is a smart pointer to the FormDetectionProcessor object - it used for the object detection (recognition) and (if it provided by the implementation) tracking
static std::shared_ptr<FormDetectionProcessor> form_detection_processor;
/// This is old detected ROI - used for smoothing an behavior of the detection rectangle on the frame
static cv::Rect old_rectangle(0, 0, 0, 0);

/// this is an object with a speed value (here is not an actual speed, this is values for creating speed commands)
static std::string current_speed = "Speed LW=0, RW=0";

#endif //IRON_TURTLE_REAR_SIGHT_WEBRTC_MANIPULATION_H
