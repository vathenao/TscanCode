ts_add_definitions(
	-DTSCANCODELIB_IMPORT
	-DTSCOMMONLIB_IMPORT
	-D_CRT_SECURE_NO_WARNINGS
	-DWIN32
	-DWIN32_LEAN_AND_MEAN	
)

ts_include_directories(
.
../include/tscore
../include/common
)

ts_add_sources(
	cmdlineparser.cpp
	cmdlineparser.h
	main.cpp
	tscexecutor.cpp
	tscexecutor.h
	tscthreadexecutor.cpp
	tscthreadexecutor.h
)

ts_add_resources(${TS_BIN_TARGET_PATH}/cfg
	../cfg/avr.cfg
	../cfg/cfg.xml
	../cfg/gnu.cfg
	../cfg/gtk.cfg
	../cfg/microsoft_sal.cfg
	../cfg/posix.cfg
	../cfg/qt.cfg
	../cfg/sdl.cfg
	../cfg/std.cfg
	../cfg/windows.cfg
)

ts_add_executable(tscancode ${TS_BIN_TARGET_PATH})

ts_link_libraries(
	tscore
)