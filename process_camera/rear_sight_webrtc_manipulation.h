//
// Created by biba_bo on 2020-08-21.
//

#ifndef IRON_TURTLE_REAR_SIGHT_WEBRTC_MANIPULATION_H
#define IRON_TURTLE_REAR_SIGHT_WEBRTC_MANIPULATION_H

#include <locale.h>
#include <glib.h>
#include <gst/gst.h>
#include <gst/sdp/sdp.h>

#ifdef G_OS_UNIX
#include <glib-unix.h>
#endif

#define GST_USE_UNSTABLE_API
#include <gst/webrtc/webrtc.h>

#include <libsoup/soup.h>
#include <json-glib/json-glib.h>
#include <string.h>

#include <opencv2/opencv.hpp>
#include <iostream>

/*This ia a payload server for possibility to establish client-server connection
 * I tried to use a local payload server, but I had difficulties with configuring it.*/
#define RTP_PAYLOAD_TYPE "96"
#define SOUP_HTTP_PORT 57778
#define STUN_SERVER "stun.l.google.com:19302"
//#define STUN_SERVER "127.0.0.1:3478"

//Struct which contains pipeline and other GstElements, for comfortable work with connection.
typedef struct _ReceiverEntry ReceiverEntry;

#include "rear_sight_processor/image_processing.h"
#include "rear_sight_processor/rear_sight_processor.h"

//Struct which contains pipeline and other GstElements, for comfortable work with connection.
typedef struct _ReceiverEntry ReceiverEntry;

static GstPadProbeReturn cb_have_data (GstPad *pad, GstPadProbeInfo *info, gpointer user_data);

ReceiverEntry* create_receiver_entry();
void destroy_receiver_entry (gpointer receiver_entry_ptr);
void on_offer_created_cb (GstPromise * promise, gpointer user_data);
void on_negotiation_needed_cb (GstElement * webrtcbin, gpointer user_data);
void on_ice_candidate_cb (G_GNUC_UNUSED GstElement * webrtcbin, guint mline_index, gchar * candidate, gpointer user_data);
static gchar * get_string_from_json_object (JsonObject * object);


std::shared_ptr<FrameParameters> frame_param = std::make_shared<FrameParameters>();
std::shared_ptr<RearSightProcessor> rear_sight_processor = std::make_shared<RearSightProcessor>(frame_param);

#endif //IRON_TURTLE_REAR_SIGHT_WEBRTC_MANIPULATION_H
