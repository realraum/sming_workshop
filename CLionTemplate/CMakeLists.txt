cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


STRING(APPEND SMING_HOME $ENV{SMING_HOME})
STRING(APPEND SMING_LIB ${SMING_HOME} "Libraries")
STRING(APPEND SMING_SYS_INC ${SMING_HOME} "system/include")
STRING(APPEND ESP_SDK_INC $ENV{ESP_HOME} "/sdk/include")

include_directories("include" ${SMING_HOME} ${SMING_LIB} ${SMING_SYS_INC} ${ESP_SDK_INC})

set(SOURCE_FILES app/application.cpp include/user_config.h)
add_executable(dummy ${SOURCE_FILES})

message("SMING_HOME: ${SMING_HOME}")
message("SMING_LIB: ${SMING_LIB}")
message("SMING_SYS_INC: ${SMING_SYS_INC}")
message("ESP_SDK_INC: ${ESP_SDK_INC}")