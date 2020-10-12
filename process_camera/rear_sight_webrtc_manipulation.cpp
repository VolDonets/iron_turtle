//
// Created by biba_bo on 2020-08-21.
//

#include "rear_sight_webrtc_manipulation.h"

void init_rear_sight_processor() {
    frame_param = std::make_shared<FrameParameters>();
    rear_sight_processor = std::make_shared<RearSightProcessor>(frame_param);
    count_frames = 0;
    //form_detection_processor = std::make_shared<FormDetectionProcessor>();
}

/// a GstPad callback function, it is used for modification a pipeline stream
static GstPadProbeReturn cb_have_data (GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
    GstMapInfo map;
    GstBuffer *buffer;

    buffer = GST_PAD_PROBE_INFO_BUFFER (info);

    buffer = gst_buffer_make_writable (buffer);

    /* Making a buffer writable can fail (for example if it
     * cannot be copied and is used more than once)
     */
    if (buffer == NULL)
        return GST_PAD_PROBE_OK;

    if (gst_buffer_map (buffer, &map, GST_MAP_WRITE)) {
        cv::Mat done_main_image, done_mini_image;
        cv::Size frame_size(WIDTH, HEIGHT);

        cv::Mat main_image = cv::Mat(frame_size, CV_8UC4, (char*)(map.data), cv::Mat::AUTO_STEP);

//        if (count_frames == CHECK_PER_FRAMES) {
//            cv::Mat checking_mat = main_image.clone();
//            form_detection_processor->add_frame(checking_mat);
//            count_frames = 0;
//        }
//        count_frames++;
//        std::vector<cv::Rect> *faces_coord = form_detection_processor->getLastDetectedFaces();
//        if (faces_coord != nullptr) {
//            // this line draw all detected ROIs, WITHOUT interpolation
//            for(int i = 0; i < faces_coord->size(); i++)
//                cv::rectangle(main_image, faces_coord->operator[](i), cv::Scalar(0, 255, 0), 2, 0, 0);

            // this lines draw only one ROI with detected object, but with interpolation.
            /* std::cout << "Size::: " << faces_coord->size() << "\n";
            if (faces_coord->size() != 0) {
                if (old_rectangle.width == 0)
                    old_rectangle = faces_coord->operator[](0);
                old_rectangle = cv::Rect(faces_coord->operator[](0).x * INTERPOLATION_COEFFICIENT + old_rectangle.x * (1 - INTERPOLATION_COEFFICIENT),
                                          faces_coord->operator[](0).y * INTERPOLATION_COEFFICIENT + old_rectangle.y * (1 - INTERPOLATION_COEFFICIENT),
                                          faces_coord->operator[](0).width * INTERPOLATION_COEFFICIENT + old_rectangle.width * (1 - INTERPOLATION_COEFFICIENT),
                                          faces_coord->operator[](0).height * INTERPOLATION_COEFFICIENT + old_rectangle.height * (1 - INTERPOLATION_COEFFICIENT));

                cv::rectangle(main_image, old_rectangle, cv::Scalar(0, 255, 0), 2, 0, 0);
            } else
                old_rectangle.width = 0;
                */
//        }


        cv::Mat copy_main_image = main_image.clone();

        cv::Rect my_interest_region(frame_param->CROPPED_X, frame_param->CROPPED_Y,
                                    frame_param->CROPPED_WIDTH, frame_param->CROPPED_HEIGHT);
        cv::rectangle(copy_main_image, my_interest_region, cv::Scalar(0, 0,255), 2, 0, 0);

        cv::Mat cropped_img = main_image(my_interest_region);
        cv::resize(cropped_img, done_main_image, cv::Size(WIDTH, HEIGHT));
        cv::resize(copy_main_image, done_mini_image, cv::Size(RESIZE_WIDTH, RESIZE_HEIGHT));

        cv::Rect main_insertion_coord(0, 0, WIDTH, HEIGHT);
        cv::Rect mini_insertion_coord(RESIZE_X, RESIZE_Y, RESIZE_WIDTH, RESIZE_HEIGHT);

        done_main_image.copyTo(main_image(main_insertion_coord));
        done_mini_image.copyTo(main_image(mini_insertion_coord));

        cv::circle(main_image, cv::Point(DRAW_CIRCLE_X, DRAW_CIRCLE_Y), DRAW_CIRCLE_RADIUS, cv::Scalar(0, 0, 255), 2, cv::LINE_8, 0);
        cv::line(main_image, cv::Point(DRAW_LINE_1B_X, DRAW_LINE_1B_Y), cv::Point(DRAW_LINE_1E_X, DRAW_LINE_1E_Y), cv::Scalar(0, 0, 255), 2, cv::LINE_8);
        cv::line(main_image, cv::Point(DRAW_LINE_2B_X, DRAW_LINE_2B_Y), cv::Point(DRAW_LINE_2E_X, DRAW_LINE_2E_Y), cv::Scalar(0, 0, 255), 2, cv::LINE_8);
        cv::line(main_image, cv::Point(DRAW_LINE_3B_X, DRAW_LINE_3B_Y), cv::Point(DRAW_LINE_3E_X, DRAW_LINE_3E_Y), cv::Scalar(0, 0, 255), 2, cv::LINE_8);
        cv::line(main_image, cv::Point(DRAW_LINE_4B_X, DRAW_LINE_4B_Y), cv::Point(DRAW_LINE_4E_X, DRAW_LINE_4E_Y), cv::Scalar(0, 0, 255), 2, cv::LINE_8);

        // printing a speed value (here is a commands for the left and the right wheels)
        cv::putText(main_image, current_speed, cv::Point(5, 15), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
        gst_buffer_unmap (buffer, &map);
    }

    GST_PAD_PROBE_INFO_DATA (info) = buffer;

    return GST_PAD_PROBE_OK;
}

//a function fro filling a ReceiverEntry structure
//Here creates a pipeline, and adds a callback function for stream modifications
ReceiverEntry* create_receiver_entry (seasocks::WebSocket * connection) {
    std::cout << "receiver entry created" << "\n";
    GError *error;
    ReceiverEntry *receiver_entry;
    GstWebRTCRTPTransceiver *trans;
    GArray *transceivers;

    receiver_entry = static_cast<ReceiverEntry *>(g_slice_alloc0(sizeof(ReceiverEntry)));
    receiver_entry->connection = connection;

    //g_object_ref (G_OBJECT (connection));

    #ifdef RASPBERRY_PI
    error = NULL;
    receiver_entry->pipeline =
                gst_parse_launch ("webrtcbin name=webrtcbin  stun-server=stun://" STUN_SERVER " "
                                  "v4l2src device=/dev/video0 "
                                  "! video/x-raw,width=" STR_WIDTH ",height=" STR_HEIGHT ",framerate=" STR_FRAMERATE " "
                                  "! videoconvert name=ocvvideosrc "
                                  "! video/x-raw,format=BGRA "
                                  "! videoconvert "
                                  "! queue max-size-buffers=1 "
                                  "! omxh264enc "
                                  "! queue max-size-time=100000000 "
                                  "! rtph264pay config-interval=10 name=payloader pt=96 "
                                  "! capssetter caps=\"application/x-rtp,profile-level-id=(string)42c01f,media=(string)video,encoding-name=(string)H264,payload=(int)96\" "
                                  "! webrtcbin. ", &error);
    #endif //RASPBERRY_PI

    #ifdef UBUNTU_PC
    error = NULL;
    receiver_entry->pipeline =
                gst_parse_launch ("webrtcbin name=webrtcbin  stun-server=stun://" STUN_SERVER " "
                                  "v4l2src device=/dev/video0 "
                                  "! video/x-raw,width=" STR_WIDTH ",height=" STR_HEIGHT ",framerate=" STR_FRAMERATE " "
                                  "! videoconvert name=ocvvideosrc "
                                  "! video/x-raw,format=BGRA "
                                  "! videoconvert "
                                  "! queue max-size-buffers=1 "
                                  "! x264enc speed-preset=ultrafast tune=zerolatency key-int-max=15 "
                                  "! video/x-h264,profile=constrained-baseline "
                                  "! queue max-size-time=0 "
                                  "! h264parse "
                                  "! rtph264pay config-interval=-1 name=payloader "
                                  "! application/x-rtp,media=video,encoding-name=H264,payload=" RTP_PAYLOAD_TYPE " "
                                  "! webrtcbin. ", &error);
    #endif //UBUNTU_PC


    if (error != NULL) {
        g_error ("Could not create WebRTC pipeline: %s\n", error->message);
        g_error_free (error);
        goto cleanup;
    }

    receiver_entry->ocvvideosrc = gst_bin_get_by_name(GST_BIN(receiver_entry->pipeline), "ocvvideosrc");
    GstPad *pad;
    pad = gst_element_get_static_pad(receiver_entry->ocvvideosrc, "src");
    gst_pad_add_probe(pad, GST_PAD_PROBE_TYPE_BUFFER, (GstPadProbeCallback)cb_have_data, NULL, NULL);
    gst_object_unref(pad);

    receiver_entry->webrtcbin =
            gst_bin_get_by_name (GST_BIN (receiver_entry->pipeline), "webrtcbin");
    g_assert (receiver_entry->webrtcbin != NULL);

    g_signal_emit_by_name (receiver_entry->webrtcbin, "get-transceivers",
                           &transceivers);
    g_assert (transceivers != NULL && transceivers->len > 0);
    trans = g_array_index (transceivers, GstWebRTCRTPTransceiver *, 0);
    trans->direction = GST_WEBRTC_RTP_TRANSCEIVER_DIRECTION_SENDONLY;
    g_array_unref (transceivers);

    g_signal_connect (receiver_entry->webrtcbin, "on-negotiation-needed",
                      G_CALLBACK (on_negotiation_needed_cb), (gpointer) receiver_entry);

    g_signal_connect (receiver_entry->webrtcbin, "on-ice-candidate",
                      G_CALLBACK (on_ice_candidate_cb), (gpointer) receiver_entry);

    gst_element_set_state (receiver_entry->pipeline, GST_STATE_PLAYING);

    return receiver_entry;

    cleanup:
    destroy_receiver_entry ((gpointer) receiver_entry);
    return NULL;
}

void destroy_receiver_entry (gpointer receiver_entry_ptr) {
    ReceiverEntry *receiver_entry = (ReceiverEntry *) receiver_entry_ptr;

    g_assert (receiver_entry != NULL);

    if (receiver_entry->pipeline != NULL) {
        gst_element_set_state (GST_ELEMENT (receiver_entry->pipeline),
                               GST_STATE_NULL);

        gst_object_unref (GST_OBJECT (receiver_entry->webrtcbin));
        gst_object_unref (GST_OBJECT (receiver_entry->pipeline));
    }

    // if (receiver_entry->connection != NULL)
    //    g_object_unref (G_OBJECT (receiver_entry->connection));
    if (receiver_entry->ocvvideosrc != NULL)
        g_object_unref(G_OBJECT((receiver_entry->ocvvideosrc)));

    g_slice_free1 (sizeof (ReceiverEntry), receiver_entry);
}


void on_offer_created_cb (GstPromise * promise, gpointer user_data) {
    gchar *sdp_string;
    gchar *json_string;
    JsonObject *sdp_json;
    JsonObject *sdp_data_json;
    GstStructure const *reply;
    GstPromise *local_desc_promise;
    GstWebRTCSessionDescription *offer = NULL;
    ReceiverEntry *receiver_entry = (ReceiverEntry *) user_data;

    reply = gst_promise_get_reply (promise);
    gst_structure_get (reply, "offer", GST_TYPE_WEBRTC_SESSION_DESCRIPTION,
                       &offer, NULL);
    gst_promise_unref (promise);

    local_desc_promise = gst_promise_new ();
    g_signal_emit_by_name (receiver_entry->webrtcbin, "set-local-description",
                           offer, local_desc_promise);
    gst_promise_interrupt (local_desc_promise);
    gst_promise_unref (local_desc_promise);

    sdp_string = gst_sdp_message_as_text (offer->sdp);
    g_print ("Negotiation offer created:\n%s\n", sdp_string);

    sdp_json = json_object_new ();
    json_object_set_string_member (sdp_json, "type", "sdp");

    sdp_data_json = json_object_new ();
    json_object_set_string_member (sdp_data_json, "type", "offer");
    json_object_set_string_member (sdp_data_json, "sdp", sdp_string);
    json_object_set_object_member (sdp_json, "data", sdp_data_json);

    json_string = get_string_from_json_object (sdp_json);
    json_object_unref (sdp_json);

    receiver_entry->connection->send(json_string);
    g_free (json_string);
    g_free (sdp_string);

    gst_webrtc_session_description_free (offer);
}


void on_negotiation_needed_cb (GstElement * webrtcbin, gpointer user_data) {
    GstPromise *promise;
    ReceiverEntry *receiver_entry = (ReceiverEntry *) user_data;

    g_print ("Creating negotiation offer\n");

    promise = gst_promise_new_with_change_func (on_offer_created_cb,
                                                (gpointer) receiver_entry, NULL);
    g_signal_emit_by_name (G_OBJECT (webrtcbin), "create-offer", NULL, promise);
}


void on_ice_candidate_cb (G_GNUC_UNUSED GstElement * webrtcbin, guint mline_index, gchar * candidate, gpointer user_data) {
    JsonObject *ice_json;
    JsonObject *ice_data_json;
    gchar *json_string;
    ReceiverEntry *receiver_entry = (ReceiverEntry *) user_data;

    ice_json = json_object_new ();
    json_object_set_string_member (ice_json, "type", "ice");

    ice_data_json = json_object_new ();
    json_object_set_int_member (ice_data_json, "sdpMLineIndex", mline_index);
    json_object_set_string_member (ice_data_json, "candidate", candidate);
    json_object_set_object_member (ice_json, "data", ice_data_json);

    json_string = get_string_from_json_object (ice_json);
    json_object_unref (ice_json);

    receiver_entry->connection->send(json_string);
    g_free (json_string);
}


void webrtc_session_handle ( const char * data_string) {
    const gchar *type_string;
    JsonNode *root_json;
    JsonObject *root_json_object;
    JsonObject *data_json_object;
    JsonParser *json_parser = NULL;
    ReceiverEntry *receiver_entry = my_receiver_entry;

    json_parser = json_parser_new ();
    if (!json_parser_load_from_data (json_parser, data_string, -1, NULL))
        goto unknown_message;

    root_json = json_parser_get_root (json_parser);
    if (!JSON_NODE_HOLDS_OBJECT (root_json))
        goto unknown_message;

    root_json_object = json_node_get_object (root_json);

    if (!json_object_has_member (root_json_object, "type")) {
        g_error ("Received message without type field\n");
        goto cleanup;
    }
    type_string = json_object_get_string_member (root_json_object, "type");
    if (!json_object_has_member (root_json_object, "data")) {
        g_error ("Received message without data field\n");
        goto cleanup;
    }
    data_json_object = json_object_get_object_member (root_json_object, "data");

    if (g_strcmp0 (type_string, "sdp") == 0) {
        const gchar *sdp_type_string;
        const gchar *sdp_string;
        GstPromise *promise;
        GstSDPMessage *sdp;
        GstWebRTCSessionDescription *answer;
        int ret;

        if (!json_object_has_member (data_json_object, "type")) {
            g_error ("Received SDP message without type field\n");
            goto cleanup;
        }
        sdp_type_string = json_object_get_string_member (data_json_object, "type");

        if (g_strcmp0 (sdp_type_string, "answer") != 0) {
            g_error ("Expected SDP message type \"answer\", got \"%s\"\n",
                     sdp_type_string);
            goto cleanup;
        }

        if (!json_object_has_member (data_json_object, "sdp")) {
            g_error ("Received SDP message without SDP string\n");
            goto cleanup;
        }
        sdp_string = json_object_get_string_member (data_json_object, "sdp");

        g_print ("Received SDP:\n%s\n", sdp_string);

        ret = gst_sdp_message_new (&sdp);
        g_assert_cmphex (ret, ==, GST_SDP_OK);

        ret =
                gst_sdp_message_parse_buffer ((guint8 *) sdp_string,
                                              strlen (sdp_string), sdp);
        if (ret != GST_SDP_OK) {
            g_error ("Could not parse SDP string\n");
            goto cleanup;
        }

        answer = gst_webrtc_session_description_new (GST_WEBRTC_SDP_TYPE_ANSWER,
                                                     sdp);
        g_assert_nonnull (answer);

        promise = gst_promise_new ();
        g_signal_emit_by_name (receiver_entry->webrtcbin, "set-remote-description",
                               answer, promise);
        gst_promise_interrupt (promise);
        gst_promise_unref (promise);
        gst_webrtc_session_description_free (answer);
    } else if (g_strcmp0 (type_string, "ice") == 0) {
        guint mline_index;
        const gchar *candidate_string;

        if (!json_object_has_member (data_json_object, "sdpMLineIndex")) {
            g_error ("Received ICE message without mline index\n");
            goto cleanup;
        }
        mline_index =
                json_object_get_int_member (data_json_object, "sdpMLineIndex");

        if (!json_object_has_member (data_json_object, "candidate")) {
            g_error ("Received ICE message without ICE candidate string\n");
            goto cleanup;
        }
        candidate_string = json_object_get_string_member (data_json_object,
                                                          "candidate");

        g_print ("Received ICE candidate with mline index %u; candidate: %s\n",
                 mline_index, candidate_string);

        g_signal_emit_by_name (receiver_entry->webrtcbin, "add-ice-candidate",
                               mline_index, candidate_string);
    } else
        goto unknown_message;

    cleanup:
   if (json_parser != NULL)
        g_object_unref (G_OBJECT (json_parser));

   return ;

    unknown_message:
    std::cerr << "Unknown message " << data_string << ", ignoring\n";
    goto cleanup;
}


static gchar * get_string_from_json_object (JsonObject * object) {
    JsonNode *root;
    JsonGenerator *generator;
    gchar *text;

    /* Make it the root node */
    root = json_node_init_object (json_node_alloc (), object);
    generator = json_generator_new ();
    json_generator_set_root (generator, root);
    text = json_generator_to_data (generator, NULL);

    /* Release everything */
    g_object_unref (generator);
    json_node_free (root);
    return text;
}

// TODO. Fix a bug when the program freezes instead of the stopping when customer is trying to stop (quit) the program using the '^C' command
// I should to fix it in the move beautiful way (now I just comment the freezes code blocks)
/* #ifdef G_OS_UNIX
gboolean exit_sighandler (gpointer user_data) {
    g_print ("Caught signal, stopping mainloop\n");
    GMainLoop *mainloop = (GMainLoop *) user_data;
    g_main_loop_quit (mainloop);
    return TRUE;
}
#endif */
// ***< Fix a bug when the program freezes instead of the stopping when customer is trying to stop (quit) the program using the '^C' command
// I should to fix it in the move beautiful way (now I just comment the freezes code blocks)

int webrtc_gst_loop(seasocks::WebSocket *connection) {
    setlocale(LC_ALL, "");
    gst_init(nullptr, nullptr);

    mainloop = g_main_loop_new(NULL, FALSE);
    g_assert (mainloop != NULL);

// TODO. Fix a bug when the program freezes instead of the stopping when customer is trying to stop (quit) the program using the '^C' command
// I should to fix it in the move beautiful way (now I just comment the freezes code blocks)
/*#ifdef G_OS_UNIX
    g_unix_signal_add(SIGINT, exit_sighandler, mainloop);
    g_unix_signal_add(SIGTERM, exit_sighandler, mainloop);
#endif */
// ***< Fix a bug when the program freezes instead of the stopping when customer is trying to stop (quit) the program using the '^C' command
// I should to fix it in the move beautiful way (now I just comment the freezes code blocks)

    my_receiver_entry = create_receiver_entry(connection);
#ifdef MY_DEBUG
    g_print("The mainloop is starting\n");
#endif //MY_DEBUG
    g_main_loop_run(mainloop);
#ifdef MY_DEBUG
    g_print("The mainloop is stopping\n");
#endif //MY_DEBUG
    g_main_loop_unref(mainloop);
#ifdef MY_DEBUG
    g_print("Gst is deiniting\n");
#endif //MY_DEBUG
    // TODO. Fix a bug when the program freezes instead of the stopping when customer is trying to stop (quit) the program using the '^C' command
    // I should to fix it in the move beautiful way (now I just comment the freezes code blocks)
    //gst_deinit();
    // ***< Fix a bug when the program freezes instead of the stopping when customer is trying to stop (quit) the program using the '^C' command
    // I should to fix it in the move beautiful way (now I just comment the freezes code blocks)
#ifdef  MY_DEBUG
    g_print("Gst stopped successfully\n");
#endif //MY_DEBUG
    return 0;
}

void webrtc_pipeline_restart(seasocks::WebSocket *connection) {
    my_receiver_entry = create_receiver_entry(connection);
}

void webrtc_pipeline_deactivate(seasocks::WebSocket *connection) {
    if (my_receiver_entry->connection != connection)
        return;
    gst_element_set_state(my_receiver_entry->pipeline, GST_STATE_NULL);
    destroy_receiver_entry(my_receiver_entry);
}

void on_zoom_plus_processor() {
    rear_sight_processor->on_zoom_plus_processor();
    rear_sight_processor->set_new_frame_param();
}

void on_zoom_minus_processor() {
    rear_sight_processor->on_zoom_minus_processor();
    rear_sight_processor->set_new_frame_param();
}

void on_move_left_processor() {
    rear_sight_processor->on_move_left_processor();
    rear_sight_processor->set_new_frame_param();
}

void on_move_right_processor() {
    rear_sight_processor->on_move_right_processor();
    rear_sight_processor->set_new_frame_param();
}

void on_move_up_processor() {
    rear_sight_processor->on_move_up_processor();
    rear_sight_processor->set_new_frame_param();
}

void on_move_down_processor() {
    rear_sight_processor->on_move_down_processor();
    rear_sight_processor->set_new_frame_param();
}

void set_speed_values_gst_pipeline_info(double currentLeftSpeed, double currentRightSpeed) {
    current_speed = "Speed LW=" + std::to_string((int)(currentLeftSpeed * 1000))
            + ", RW=" + std::to_string((int)(currentRightSpeed * 1000));
}
