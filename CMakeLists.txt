cmake_minimum_required(VERSION 3.17)
project(iron_turtle)

set(CMAKE_CXX_STANDARD 20)

add_executable(iron_turtle main.cpp
        web_server/web_server.h
        web_server/web_server.cpp

        web_server_worker.h
        web_server_worker.cpp

        web_server/lib_my_event_handler/delegate_ws.h
        web_server/lib_my_event_handler/delegate_ws.cpp
        web_server/lib_my_event_handler/event_ws.h
        web_server/lib_my_event_handler/event_ws.cpp
        web_server/lib_my_event_handler/handler_ws.h

        turtle_manager/ascii_com_manager/ascii_com_lim.h
        turtle_manager/ascii_com_manager/ascii_native_driver.h
        turtle_manager/ascii_com_manager/ascii_native_driver.cpp

        turtle_manager/turtle_manager.h
        turtle_manager/turtle_manager.cpp

        turtle_manager/ascii_com_manager/ascii_native_driver_posn_save.cpp
        turtle_manager/ascii_com_manager/ascii_native_driver_posn_save.h
        turtle_manager/ascii_com_manager/serial_manager.cpp
        turtle_manager/ascii_com_manager/serial_manager.h

        process_camera/rear_sight_processor/image_processing.h
        process_camera/rear_sight_processor/rear_sight_processor.h
        process_camera/rear_sight_processor/rear_sight_processor_keep_frame_proportionality.cpp
        process_camera/rear_sight_webrtc_manipulation.cpp
        process_camera/rear_sight_webrtc_manipulation.h

#        process_camera/form_detector_ocv/form_detection_processor_haarcascade.cpp
#        process_camera/form_detector_ocv/form_detection_processor_dnn_caffe.cpp
#        process_camera/form_detector_ocv/form_detection_processor_yolov3_tiny.cpp
        process_camera/form_detector_ocv/form_det_track_svm.cpp
        process_camera/form_detector_ocv/form_detection_processor.h)

#target_compile_definitions(iron_turtle PRIVATE HAARCASCADE FRONTAL_FACE)
#target_compile_definitions(iron_turtle PRIVATE HAARCASCADE ANOTHER_HAND)
#target_compile_definitions(iron_turtle PRIVATE HAARCASCADE FIST)
#target_compile_definitions(iron_turtle PRIVATE HAARCASCADE RPALM)

#target_compile_definitions(iron_turtle PRIVATE YOLO3_TINY YOLO3_TINY_COCO)
#target_compile_definitions(iron_turtle PRIVATE YOLO3_TINY CROSS_HANDS)
#target_compile_definitions(iron_turtle PRIVATE YOLO3_TINY CROSS_HANDS_PRN)

#target_compile_definitions(iron_turtle PRIVATE DNN_CAFFE BY_BALD_GUY)
#target_compile_definitions(iron_turtle PRIVATE DNN_CAFFE FROM_YOLO)

#target_compile_definitions(iron_turtle PRIVATE SVM_HOG V8_C8)
#target_compile_definitions(iron_turtle PRIVATE SVM_HOG V6_C20)
#target_compile_definitions(iron_turtle PRIVATE SVM_HOG V10_C10)
#target_compile_definitions(iron_turtle PRIVATE SVM_HOG V11_C15_SUPER)
#target_compile_definitions(iron_turtle PRIVATE SVM_HOG V12_C15_SUPER)
target_compile_definitions(iron_turtle PRIVATE SVM_HOG V14_C15_SUPER)

target_link_libraries(iron_turtle /lib/x86_64-linux-gnu/libgsl.so.23)
target_link_libraries(iron_turtle /lib/x86_64-linux-gnu/libgslcblas.so.0)
target_link_libraries(iron_turtle /lib/x86_64-linux-gnu/liblapack.so)

target_link_libraries(iron_turtle seasocks "${ZLIB_LIBRARIES}")
target_link_libraries(iron_turtle pthread z)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(iron_turtle ${OpenCV_LIBS})

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

add_custom_command(TARGET iron_turtle POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/src $<TARGET_FILE_DIR:iron_turtle>/src)