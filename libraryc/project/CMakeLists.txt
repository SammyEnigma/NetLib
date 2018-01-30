cmake_minimum_required(VERSION 2.8.12)

project(CoreLibC)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(./headers)

set(PROJECT_SOURCE_DIR ./sources)
set(PROJECT_HEADER_DIR ./headers)

set(SOURCE
${SOURCE}
${PROJECT_SOURCE_DIR}/NodeC.c
${PROJECT_SOURCE_DIR}/KeyValueC.c
${PROJECT_SOURCE_DIR}/TreeC.c
${PROJECT_SOURCE_DIR}/ListC.c
${PROJECT_SOURCE_DIR}/MapC.c
${PROJECT_SOURCE_DIR}/QueueC.c
${PROJECT_SOURCE_DIR}/StackC.c
${PROJECT_SOURCE_DIR}/CoreLib.c
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
${PROJECT_HEADER_DIR}/CoreLib.h
)

add_library(${PROJECT_NAME}  ${SOURCE} ${HEADERS})

