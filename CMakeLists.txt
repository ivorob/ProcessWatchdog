CMAKE_MINIMUM_REQUIRED (VERSION 3.0)
PROJECT (ProcessWatchdog)

SET (SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET (SOURCES
        ${SRC_DIR}/ProcessWatchdog.cpp
    )

SET (HEADERS
        ${SRC_DIR}/ProcessWatchdog.h
    )

ADD_DEFINITIONS (-DUNICODE -D_UNICODE)

ADD_LIBRARY(libProcessWatchdog ${SOURCES} ${HEADERS})
TARGET_INCLUDE_DIRECTORIES (libProcessWatchdog PUBLIC ${SRC_DIR})

ADD_SUBDIRECTORY (examples)

