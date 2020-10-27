cmake_minimum_required(VERSION 3.13)
project(iron_turtle)

set(CMAKE_CXX_STANDARD 20)

add_executable(iron_turtle
###      source file with the project begin function
        main.cpp

###      web server
        web_server/web_server.h
        web_server/web_server.cpp

###      class for the handling process in other thread for the WebServer events
        web_server_worker.h
        web_server_worker.cpp

###      my own handling sub system
        web_server/lib_my_event_handler/delegate_ws.h
        web_server/lib_my_event_handler/delegate_ws.cpp
        web_server/lib_my_event_handler/event_ws.h
        web_server/lib_my_event_handler/event_ws.cpp
        web_server/lib_my_event_handler/handler_ws.h

###      binary protocol wrapper
        turtle_manager/binary_com_manager/bipropellant-api/hbprotocol/protocol.h
        turtle_manager/binary_com_manager/bipropellant-api/hbprotocol/protocol_private.h
        turtle_manager/binary_com_manager/bipropellant-api/hbprotocol/ascii_protocol.h

        turtle_manager/binary_com_manager/bipropellant-api/hbprotocol/protocol.c
        turtle_manager/binary_com_manager/bipropellant-api/hbprotocol/machine_protocol.c
        turtle_manager/binary_com_manager/bipropellant-api/hbprotocol/ascii_protocol.c

        turtle_manager/binary_com_manager/bipropellant-api/protocolFunctions.h
        turtle_manager/binary_com_manager/bipropellant-api/protocolFunctions.c
        turtle_manager/binary_com_manager/bipropellant-api/HoverboardAPI.h
        turtle_manager/binary_com_manager/bipropellant-api/HoverboardAPI.cpp

        turtle_manager/binary_com_manager/binary_com_lim.h
        turtle_manager/binary_com_manager/serial_manager.h
        turtle_manager/binary_com_manager/serial_manager.cpp

        turtle_manager/smooth_turtle_manager.h
        turtle_manager/smooth_turtle_manager.cpp

        pursuit_processor/pursuit_processor.h
        pursuit_processor/pursuit_processor.cpp

###      this is a class for connecting to realsense camera and the depth processing
        process_camera/intel_realsense_camera/realsense_camera_processor.h
        process_camera/intel_realsense_camera/realsense_camera_processor.cpp

###      this is the source code for the image processing in the gst pipeline
        process_camera/rear_sight_processor/image_processing.h
        process_camera/rear_sight_processor/rear_sight_processor.h
        process_camera/rear_sight_processor/rear_sight_processor_keep_frame_proportionality.cpp
        process_camera/rear_sight_webrtc_manipulation.cpp
        process_camera/rear_sight_webrtc_manipulation.h

###       this is the source code for the object detection in a frame and object tracking
#        process_camera/form_detector_ocv/form_detection_processor_haarcascade.cpp
#        process_camera/form_detector_ocv/form_detection_processor_dnn_caffe.cpp
        process_camera/form_detector_ocv/form_detection_processor_yolov3_tiny.cpp
#        process_camera/form_detector_ocv/form_det_track_svm.cpp
        process_camera/form_detector_ocv/form_detection_processor.h)

###       this is a target compile definitions for the different platfroms (both linux - the first one is my work PC under
###       Ubutnu 20.04 and the second one is Raspberry Pi3 under Rasbian)
target_compile_definitions(iron_turtle PRIVATE UBUNTU_PC)
#target_compile_definitions(iron_turtle PRIVATE RASPBERRY_PI)
#set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -latomic")

###       here is used for other camera, here is used an inter realsense camera
target_compile_definitions(iron_turtle PRIVATE INTEL_REALSENSE)

###       this is a target compile definitions for the DEBUG development steps
target_compile_definitions(iron_turtle PRIVATE MY_DEBUG)

###       this is a target compile definitions for the TESTING the pursuit module
#target_compile_definitions(iron_turtle PRIVATE MY_PURSUIT_TESTING)

###       This is compile definitions for the different haarcasades for the object detection
###       !!!WARNING!!! it works only with 'process_camera/form_detector_ocv/form_detection_processor_haarcascade.cpp'
#target_compile_definitions(iron_turtle PRIVATE HAARCASCADE FRONTAL_FACE)
#target_compile_definitions(iron_turtle PRIVATE HAARCASCADE ANOTHER_HAND)
#target_compile_definitions(iron_turtle PRIVATE HAARCASCADE FIST)
#target_compile_definitions(iron_turtle PRIVATE HAARCASCADE RPALM)

###       This is compile definitions for the different yolo pretrained models for the object detection
###       !!!WARNING!!! it works only with 'process_camera/form_detector_ocv/form_detection_processor_yolov3_tiny.cpp'
target_compile_definitions(iron_turtle PRIVATE YOLO3_TINY YOLO3_TINY_COCO)
#target_compile_definitions(iron_turtle PRIVATE YOLO3_TINY YOLO3_TINY_HAND)
#target_compile_definitions(iron_turtle PRIVATE YOLO3_TINY CROSS_HANDS)
#target_compile_definitions(iron_turtle PRIVATE YOLO3_TINY CROSS_HANDS_PRN)

###       This is compile definitions for the different caffe pretrained models for the object detection
###       !!!WARNING!!! it works only with 'process_camera/form_detector_ocv/form_detection_processor_dnn_caffe.cpp'
#target_compile_definitions(iron_turtle PRIVATE DNN_CAFFE BY_BALD_GUY)
#target_compile_definitions(iron_turtle PRIVATE DNN_CAFFE FROM_YOLO)

###       This is compile definitions for the different SVM HOG pretrained models for the object detection
###       !!!WARNING!!! it works only with 'process_camera/form_detector_ocv/form_det_track_svm.cpp'
#target_compile_definitions(iron_turtle PRIVATE SVM_HOG V8_C8)
#target_compile_definitions(iron_turtle PRIVATE SVM_HOG V6_C20)
#target_compile_definitions(iron_turtle PRIVATE SVM_HOG V10_C10)
#target_compile_definitions(iron_turtle PRIVATE SVM_HOG V11_C15_SUPER)
#target_compile_definitions(iron_turtle PRIVATE SVM_HOG V12_C15_SUPER)
#target_compile_definitions(iron_turtle PRIVATE SVM_HOG V14_C15_SUPER)

###       This target libraries used under Raspberry Pi3
#target_link_libraries(iron_turtle /usr/lib/arm-linux-gnueabihf/libgsl.so.23)
#target_link_libraries(iron_turtle /usr/lib/arm-linux-gnueabihf/libgslcblas.so.0)
#target_link_libraries(iron_turtle /usr/lib/arm-linux-gnueabihf/liblapack.so)

###       This target libraries used under my Ubuntu PC
target_link_libraries(iron_turtle /lib/x86_64-linux-gnu/libgsl.so.23)
target_link_libraries(iron_turtle /lib/x86_64-linux-gnu/libgslcblas.so.0)
target_link_libraries(iron_turtle /lib/x86_64-linux-gnu/liblapack.so)

###       Adding to the project the 'seasocks' library for the WebServer which works as WebSocket server
target_link_libraries(iron_turtle seasocks "${ZLIB_LIBRARIES}")
###       Adding to the project
target_link_libraries(iron_turtle pthread z)

###       Adding to the project the 'opencv' library it used for the frame processing (and, +- as an object detector) and as an object tracker
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(iron_turtle ${OpenCV_LIBS})

###       For possibility to use an intel realsense camera we need to use an intel api for the RealSense cameras
###       here is a camera model: D415
target_link_libraries(iron_turtle realsense2)

###       Adding to the project the 'dlib' library it used as object detector (works with the SVM HOG)
find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(DLIB "dlib-1")
    if (DLIB_FOUND)
        target_link_libraries(iron_turtle ${DLIB_LIBRARIES})
        add_definitions(${DLIB_CFLAGS} ${DLIB_CFLAGS_OTHER})
    endif()
endif()

find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(GLIB "glib-2.0")
    if (GLIB_FOUND)
        target_link_libraries(iron_turtle ${GLIB_LIBRARIES})
        add_definitions(${GLIB_CFLAGS} ${GLIB_CFLAGS_OTHER})
    endif()
endif()

find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(GST "gstreamer-1.0")
    if (GST_FOUND)
        target_link_libraries(iron_turtle ${GST_LIBRARIES})
        add_definitions(${GST_CFLAGS} ${GST_CFLAGS_OTHER})
    endif()
endif()

find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(SPD "gstreamer-sdp-1.0")
    if (SPD_FOUND)
        target_link_libraries(iron_turtle ${SPD_LIBRARIES})
        add_definitions(${SPD_CFLAGS} ${SPD_CFLAGS_OTHER})
    endif()
endif()

find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(WEBRTC "gstreamer-webrtc-1.0")
    if (WEBRTC_FOUND)
        target_link_libraries(iron_turtle ${WEBRTC_LIBRARIES})
        add_definitions(${WEBRTC_CFLAGS} ${WEBRTC_CFLAGS_OTHER})
    endif()
endif()

find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(JSON "json-glib-1.0")
    if (JSON_FOUND)
        target_link_libraries(iron_turtle ${JSON_LIBRARIES})
        add_definitions(${JSON_CFLAGS} ${JSON_CFLAGS_OTHER})
    endif()
endif()

find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(SOUP "libsoup-2.4")
    if (SOUP_FOUND)
        target_link_libraries(iron_turtle ${SOUP_LIBRARIES})
        add_definitions(${SOUP_CFLAGS} ${SOUP_CFLAGS_OTHER})
    endif()
endif()

###       Copying the source files for the runnable file
add_custom_command(TARGET iron_turtle POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/src $<TARGET_FILE_DIR:iron_turtle>/src)