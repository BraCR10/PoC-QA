cmake_minimum_required(VERSION 3.10)
project(smart_lock CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(eclipse-paho-mqtt-c REQUIRED)

add_executable(smart_lock
    main.cpp
    connectivity/network.cpp
    connectivity/mqtt.cpp
)

target_include_directories(smart_lock PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(smart_lock PRIVATE eclipse-paho-mqtt-c::paho-mqtt3c)
