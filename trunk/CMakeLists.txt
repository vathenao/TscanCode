CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

SET(TS_BIN_TARGET_PATH ${CMAKE_BINARY_DIR}/bin)
#SET(TS_CORE_TARGET_PATH ${TS_BIN_TARGET_PATH}/tscore)

PROJECT(tscancode)
include(cmake/ts.cmake)

ts_add_subdirectory(cli)
ts_add_subdirectory(lib)
ts_add_subdirectory(common)