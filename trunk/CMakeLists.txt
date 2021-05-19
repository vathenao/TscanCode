CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

SET(TS_BIN_TARGET_PATH ${CMAKE_BINARY_DIR}/bin)

PROJECT(tscan)
include(cmake/ts.cmake)

ts_add_subdirectory(common)
ts_add_subdirectory(core)
ts_add_subdirectory(tscancode)