cmake_minimum_required(VERSION 3.10.0)

project(CoreNetwork)

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
include_directories( C:/ExternLibs/usr/include/ C:/ExternLibs/usr/include/openssl/ ../librarycpp/headers)
link_directories(C:/ExternLibs/usr/lib)
else()
include_directories( /include/ /usr/include/ /usr/local/include/ /usr/include/openssl/ ../librarycpp/headers)
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
${PROJECT_SOURCE_DIR}/ABNFMessage.cpp
${PROJECT_SOURCE_DIR}/ImapClient.cpp
${PROJECT_SOURCE_DIR}/Mail.cpp
${PROJECT_SOURCE_DIR}/MailBox.cpp
${PROJECT_SOURCE_DIR}/Network.cpp
${PROJECT_SOURCE_DIR}/SmtpClient.cpp
${PROJECT_SOURCE_DIR}/TcpClient.cpp
../librarycpp/sources/Base64.cpp
)

set(HEADERS
${HEADERS}
${PROJECT_HEADER_DIR}/ABNFMessage.hpp
${PROJECT_HEADER_DIR}/HttpMime.hpp
${PROJECT_HEADER_DIR}/ImapClient.hpp
${PROJECT_HEADER_DIR}/Mail.hpp
${PROJECT_HEADER_DIR}/MailBox.hpp
${PROJECT_HEADER_DIR}/Network.hpp
${PROJECT_HEADER_DIR}/SecurityType.hpp
${PROJECT_HEADER_DIR}/SmtpClient.hpp
${PROJECT_HEADER_DIR}/TcpClient.hpp
../librarycpp/headers/Base64.hpp
)

add_library(${PROJECT_NAME}  ${SOURCE} ${HEADERS})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif (WIN32)
