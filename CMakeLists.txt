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

#add_executable(iron_turtle main_ascii_test.cpp
        turtle_manager/ascii_com_manager/ascii_com_lim.h
        turtle_manager/ascii_com_manager/ascii_native_driver.h
        turtle_manager/ascii_com_manager/ascii_native_driver.cpp
        turtle_manager/turtle_manager.h
        turtle_manager/turtle_manager.cpp)

target_link_libraries(iron_turtle seasocks "${ZLIB_LIBRARIES}")
target_link_libraries(iron_turtle pthread z)

add_custom_command(TARGET iron_turtle POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/src/server_files $<TARGET_FILE_DIR:iron_turtle>/src/server_files)