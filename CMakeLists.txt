cmake_minimum_required(VERSION 2.8)

set(CMAKE_COLOR_MAKEFILE ON)

if (WIN32)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-DWIN32_LEAN_AND_MEAN)
endif(WIN32)

SET (PEDANTIC TRUE CACHE BOOL "Determines if we should compile in pedantic mode.")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(CoreLibraryTest)

set(CoreLibraryTest_VERSION_MAJOR 1)
set(CoreLibraryTest_VERSION_MINOR 0)


set(PROJECT_SOURCE_DIR ./sources)
set(CORE_LIBRARY_HEADERS ../CoreLibrary/headers)
set(CORE_LIBRARY_SOURCES ../CoreLibrary/sources)

if (WIN32)
    include_directories(./sources/ C:/usr/include ${CORE_LIBRARY_HEADERS})
    link_directories(C:/usr/lib/)
else(WIN32)
    include_directories(./sources/ /usr/include/ ${CORE_LIBRARY_HEADERS})
    link_directories(/usr/local/lib/)
endif(WIN32)

set(SOURCE
   ${SOURCE}
   ${PROJECT_SOURCE_DIR}/main.cpp
   ${PROJECT_SOURCE_DIR}/Tests.cpp
   ${CORE_LIBRARY_SOURCES}/Base64.cpp
   ${CORE_LIBRARY_SOURCES}/Buffer.cpp
   ${CORE_LIBRARY_SOURCES}/GenericString.cpp
   ${CORE_LIBRARY_SOURCES}/TimeStamp.cpp
   ${CORE_LIBRARY_SOURCES}/Variant.cpp
   ${CORE_LIBRARY_SOURCES}/File.cpp
   ${CORE_LIBRARY_SOURCES}/Directory.cpp
   ${CORE_LIBRARY_SOURCES}/Logger.cpp
   ${CORE_LIBRARY_SOURCES}/Mutex.cpp
   ${CORE_LIBRARY_SOURCES}/Configuration.cpp
   ${CORE_LIBRARY_SOURCES}/ProcessLock.cpp
   ${CORE_LIBRARY_SOURCES}/DataConnection.cpp
   ${CORE_LIBRARY_SOURCES}/ABNFMessage.cpp
   ${CORE_LIBRARY_SOURCES}/SignalHandler.cpp
   ${CORE_LIBRARY_SOURCES}/NetworkHelper.cpp
   ${CORE_LIBRARY_SOURCES}/Responder.cpp
   ${CORE_LIBRARY_SOURCES}/ServerAIO.cpp
   ${CORE_LIBRARY_SOURCES}/ServerSIO.cpp
   #${CORE_LIBRARY_SOURCES}/ABNFAdapter.cpp
   #${CORE_LIBRARY_SOURCES}/AsynchronousSocket.cpp
   #${CORE_LIBRARY_SOURCES}/HTTPHandler.cpp
   #${CORE_LIBRARY_SOURCES}/NotificationMessage.cpp
   #${CORE_LIBRARY_SOURCES}/NotificationWriter.cpp
   #${CORE_LIBRARY_SOURCES}/Notifier.cpp
   #${CORE_LIBRARY_SOURCES}/Observer.cpp
   #${CORE_LIBRARY_SOURCES}/RequestHandler.cpp
   #${CORE_LIBRARY_SOURCES}/SMTP.cpp
   #${CORE_LIBRARY_SOURCES}/SocketEventQueue.cpp
   #${CORE_LIBRARY_SOURCES}/Thread.cpp
   #${CORE_LIBRARY_SOURCES}/OracleConnection.cpp
   #${CORE_LIBRARY_SOURCES}/PSQLConnection.cpp
   #${CORE_LIBRARY_SOURCES}/ResourceHandler.cpp
)

set(HEADERS
   ${HEADERS}
   ${PROJECT_SOURCE_DIR}/Tests.h
   ${CORE_LIBRARY_HEADERS}/Node.h
   ${CORE_LIBRARY_HEADERS}/KeyValue.h
   ${CORE_LIBRARY_HEADERS}/BinaryTree.h
   ${CORE_LIBRARY_HEADERS}/List.h
   ${CORE_LIBRARY_HEADERS}/Map.h
   ${CORE_LIBRARY_HEADERS}/Array.h
   ${CORE_LIBRARY_HEADERS}/Buffer.h
   ${CORE_LIBRARY_HEADERS}/GenericString.h
   ${CORE_LIBRARY_HEADERS}/Base64.h
   ${CORE_LIBRARY_HEADERS}/TimeStamp.h
   ${CORE_LIBRARY_HEADERS}/Variant.h
   ${CORE_LIBRARY_HEADERS}/Stack.h
   ${CORE_LIBRARY_HEADERS}/Queue.h
   ${CORE_LIBRARY_HEADERS}/Mime.h
   ${CORE_LIBRARY_HEADERS}/File.h
   ${CORE_LIBRARY_HEADERS}/Directory.h
   ${CORE_LIBRARY_HEADERS}/Logger.h
   ${CORE_LIBRARY_HEADERS}/Mutex.h
   ${CORE_LIBRARY_HEADERS}/Configuration.h
   ${CORE_LIBRARY_HEADERS}/ProcessLock.h
   ${CORE_LIBRARY_HEADERS}/DataConnection.h
   ${CORE_LIBRARY_HEADERS}/ABNFMessage.h
   ${CORE_LIBRARY_HEADERS}/SignalHandler.h
   ${CORE_LIBRARY_HEADERS}/NetworkHelper.h
   ${CORE_LIBRARY_HEADERS}/Responder.h
   ${CORE_LIBRARY_HEADERS}/ServerState.h
   ${CORE_LIBRARY_HEADERS}/ServerAIO.h
   ${CORE_LIBRARY_HEADERS}/ServerSIO.h
   #${CORE_LIBRARY_HEADERS}/ResourceHandler.h
   #${CORE_LIBRARY_HEADERS}/ABNFAdapter.h
   #${CORE_LIBRARY_HEADERS}/HTTPHandler.h
   #${CORE_LIBRARY_HEADERS}/NotificationMessage.h
   #${CORE_LIBRARY_HEADERS}/NotificationWriter.h
   #${CORE_LIBRARY_HEADERS}/Notifier.h
   #${CORE_LIBRARY_HEADERS}/Observer.h
   #${CORE_LIBRARY_HEADERS}/RequestHandler.h
   #${CORE_LIBRARY_HEADERS}/SMTP.h
   #${CORE_LIBRARY_HEADERS}/SocketEvent.h
   #${CORE_LIBRARY_HEADERS}/SocketEventQueue.h
   #${CORE_LIBRARY_HEADERS}/Thread.h
   #${CORE_LIBRARY_HEADERS}/OracleConnection.h
   #${CORE_LIBRARY_HEADERS}/PSQLConnection.h
)

add_executable(CoreLibraryTest ${SOURCE} ${HEADERS})

if (WIN32)
    include_directories(./sources/ C:/usr/include ${CORE_LIBRARY_HEADERS})
    link_directories(C:/usr/lib/)
    target_link_libraries(CoreLibraryTest wsock32 ws2_32)
else(WIN32)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(CoreLibraryTest pthread)
endif(WIN32)
