cmake_minimum_required(VERSION 3.10)
project(smart_lock CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(smart_lock
    main.cpp
    storage/key_store.cpp
    connectivity/network.cpp
    connectivity/mqtt.cpp
)

target_include_directories(smart_lock PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
