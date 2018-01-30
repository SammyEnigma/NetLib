cmake_minimum_required(VERSION 2.8.12)

project(CoreLibTestCpp)

add_definitions(-std=gnu++0x)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(../project/headers)

set(PROJECT_SOURCE_DIR ../project/sources)
set(PROJECT_HEADER_DIR ../project/headers)

set(SOURCE
${SOURCE}
${PROJECT_SOURCE_DIR}/ABNFMessage.cpp
${PROJECT_SOURCE_DIR}/Base64.cpp
${PROJECT_SOURCE_DIR}/Buffer.cpp
${PROJECT_SOURCE_DIR}/Character.cpp
${PROJECT_SOURCE_DIR}/Configuration.cpp
${PROJECT_SOURCE_DIR}/DataConnection.cpp
${PROJECT_SOURCE_DIR}/DateTime.cpp
${PROJECT_SOURCE_DIR}/Directory.cpp
${PROJECT_SOURCE_DIR}/File.cpp
${PROJECT_SOURCE_DIR}/GenericString.cpp
${PROJECT_SOURCE_DIR}/Logger.cpp
${PROJECT_SOURCE_DIR}/Mail.cpp
${PROJECT_SOURCE_DIR}/Mutex.cpp
${PROJECT_SOURCE_DIR}/Network.cpp
${PROJECT_SOURCE_DIR}/ProcessLock.cpp
${PROJECT_SOURCE_DIR}/SignalHandler.cpp
${PROJECT_SOURCE_DIR}/TcpClient.cpp
${PROJECT_SOURCE_DIR}/TcpServer.cpp
${PROJECT_SOURCE_DIR}/Variant.cpp
./main.cpp
)

set(HEADERS
${HEADERS}
${PROJECT_HEADER_DIR}/ABNFMessage.hpp
${PROJECT_HEADER_DIR}/Array.hpp
${PROJECT_HEADER_DIR}/Base64.hpp
${PROJECT_HEADER_DIR}/BinaryTree.hpp
${PROJECT_HEADER_DIR}/Buffer.hpp
${PROJECT_HEADER_DIR}/Character.hpp
${PROJECT_HEADER_DIR}/Configuration.hpp
${PROJECT_HEADER_DIR}/DataConnection.hpp
${PROJECT_HEADER_DIR}/DateTime.hpp
${PROJECT_HEADER_DIR}/Directory.hpp
${PROJECT_HEADER_DIR}/File.hpp
${PROJECT_HEADER_DIR}/EncryptionTypes.hpp
${PROJECT_HEADER_DIR}/GenericString.hpp
${PROJECT_HEADER_DIR}/KeyValue.hpp
${PROJECT_HEADER_DIR}/List.hpp
${PROJECT_HEADER_DIR}/Logger.hpp
${PROJECT_HEADER_DIR}/Mail.hpp
${PROJECT_HEADER_DIR}/Map.hpp
${PROJECT_HEADER_DIR}/Mime.hpp
${PROJECT_HEADER_DIR}/Mutex.hpp
${PROJECT_HEADER_DIR}/Network.hpp
${PROJECT_HEADER_DIR}/NetworkDefs.hpp
${PROJECT_HEADER_DIR}/Node.hpp
${PROJECT_HEADER_DIR}/ProcessLock.hpp
${PROJECT_HEADER_DIR}/Queue.hpp
${PROJECT_HEADER_DIR}/SignalHandler.hpp
${PROJECT_HEADER_DIR}/Stack.hpp
${PROJECT_HEADER_DIR}/TcpClient.hpp
${PROJECT_HEADER_DIR}/TcpServer.hpp
${PROJECT_HEADER_DIR}/TreeNode.hpp
${PROJECT_HEADER_DIR}/Variant.hpp
)

add_executable(${PROJECT_NAME}  ${SOURCE} ${HEADERS})

