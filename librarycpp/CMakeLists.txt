cmake_minimum_required(VERSION 3.10.0)

project(CoreCpp)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
    add_definitions(-D_NOMINMAX)
endif (WIN32)

if(WIN32)
message("### WARNING: For Windows builds please adjust the location of libssl in next couple of lines")
include_directories( C:/ExternLibs/usr/include/ C:/ExternLibs/usr/include/openssl/)
link_directories(C:/ExternLibs/usr/lib)
else()
include_directories( /include/ /usr/include/ /usr/local/include/ /usr/include/openssl/)
link_directories(./lib/ /lib /usr/lib/ /usr/local/lib/ /usr/lib/x86_64-linux-gnu/ /usr/lib/arm-linux-gnu/ /usr/include/asm-generic/)
endif()

if(WIN32)
link_libraries(ws2_32 libssl libcrypto)
else()
link_libraries(dl rt pthread ssl crypto)
endif()

include_directories(./headers)

set(PROJECT_SOURCE_DIR ./sources)
set(PROJECT_HEADER_DIR ./headers)

set(SOURCE
${SOURCE}
${PROJECT_SOURCE_DIR}/Base64.cpp
${PROJECT_SOURCE_DIR}/Buffer.cpp
${PROJECT_SOURCE_DIR}/Configuration.cpp
${PROJECT_SOURCE_DIR}/DateTime.cpp
${PROJECT_SOURCE_DIR}/String.cpp
${PROJECT_SOURCE_DIR}/Logger.cpp
${PROJECT_SOURCE_DIR}/ProcessLock.cpp
${PROJECT_SOURCE_DIR}/SignalHandler.cpp
${PROJECT_SOURCE_DIR}/Variant.cpp
${PROJECT_SOURCE_DIR}/StringEx.cpp
${PROJECT_SOURCE_DIR}/CoreLib.cpp
)

set(HEADERS
${HEADERS}
${PROJECT_HEADER_DIR}/Array.hpp
${PROJECT_HEADER_DIR}/Base64.hpp
${PROJECT_HEADER_DIR}/BinaryTree.hpp
${PROJECT_HEADER_DIR}/Buffer.hpp
${PROJECT_HEADER_DIR}/Configuration.hpp
${PROJECT_HEADER_DIR}/DateTime.hpp
${PROJECT_HEADER_DIR}/String.hpp
${PROJECT_HEADER_DIR}/KeyValue.hpp
${PROJECT_HEADER_DIR}/List.hpp
${PROJECT_HEADER_DIR}/Logger.hpp
${PROJECT_HEADER_DIR}/Map.hpp
${PROJECT_HEADER_DIR}/Node.hpp
${PROJECT_HEADER_DIR}/ProcessLock.hpp
${PROJECT_HEADER_DIR}/Queue.hpp
${PROJECT_HEADER_DIR}/SignalHandler.hpp
${PROJECT_HEADER_DIR}/Stack.hpp
${PROJECT_HEADER_DIR}/TreeNode.hpp
${PROJECT_HEADER_DIR}/Variant.hpp
${PROJECT_HEADER_DIR}/VariantType.hpp
${PROJECT_HEADER_DIR}/StringEx.hpp
${PROJECT_HEADER_DIR}/CoreLib.hpp
)

add_library(${PROJECT_NAME}  ${SOURCE} ${HEADERS})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif (WIN32)
