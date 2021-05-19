ts_add_definitions(
	-DUNICODE
	-DHAVE_STRUCT_TIMESPEC
	-D_CRT_SECURE_NO_WARNINGS
	-DWIN32
	-DWIN32_LEAN_AND_MEAN	
)

ts_include_directories(
.
)

ts_add_sources(
	global.h
	crashhelp.cpp
	crashhelp.h
	dnetthread.cpp
	dnetthread.h
	filedepend.cpp
	filedepend.h
	filelister.cpp
	filelister.h
	pathmatch.cpp
	pathmatch.h
)

ts_add_library(tcommon STATIC ${TS_BIN_TARGET_PATH})