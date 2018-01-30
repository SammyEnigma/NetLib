cmake_minimum_required(VERSION 2.8.12)

project(CoreLibTestC)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(../project/headers/)

set(PROJECT_SOURCE_DIR ../project/sources)
set(PROJECT_HEADER_DIR ../project/headers)

set(SOURCE
${SOURCE}
${PROJECT_SOURCE_DIR}/NodeC.c
${PROJECT_SOURCE_DIR}/KeyValueC.c
${PROJECT_SOURCE_DIR}/TreeC.c
${PROJECT_SOURCE_DIR}/ListC.c
${PROJECT_SOURCE_DIR}/MapC.c
${PROJECT_SOURCE_DIR}/QueueC.c
${PROJECT_SOURCE_DIR}/StackC.c
./main.c
)

set(HEADERS
${HEADERS}
${PROJECT_HEADER_DIR}/NodeC.h
${PROJECT_HEADER_DIR}/KeyValueC.h
${PROJECT_HEADER_DIR}/TreeC.h
${PROJECT_HEADER_DIR}/ListC.h
${PROJECT_HEADER_DIR}/MapC.h
${PROJECT_HEADER_DIR}/QueueC.h
${PROJECT_HEADER_DIR}/StackC.h
)

add_executable(${PROJECT_NAME}  ${SOURCE} ${HEADERS})

