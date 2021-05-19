#msvcpch.cmake
#����һ���꣬��������
#USE_MSVC_PCH        ��������
#PCH_TARGET          ����Ŀ����
#PCH_HEADER_FILE     ��Ԥ����ͷ�ļ����ƣ�stdafx.h��
#PCH_SOURCE_FILE     ��Ԥ����Դ�ļ����ƣ�stdafx.cpp��
MACRO(USE_MSVC_PCH PCH_TARGET PCH_HEADER_FILE PCH_SOURCE_FILE)
IF(MSVC)
	# ��ȡԤ����ͷ�ļ����ļ�����ͨ����stdafx
	GET_FILENAME_COMPONENT(PCH_NAME ${PCH_HEADER_FILE} NAME_WE)
	
	# ����Ԥ�����ļ���·��
	IF(CMAKE_CONFIGURATION_TYPES)
        # ���������ѡ�Debug/Release����·������Լ�����ѡ��
		SET(PCH_DIR "${CMAKE_CURRENT_BINARY_DIR}/PCH/${CMAKE_CFG_INTDIR}")
	ELSE(CMAKE_CONFIGURATION_TYPES)
		SET(PCH_DIR "${CMAKE_CURRENT_BINARY_DIR}/PCH")
	ENDIF(CMAKE_CONFIGURATION_TYPES)
 
	# ����Ԥ�����ļ���·��
	FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/PCH)
 
	# ������Ŀ���ԣ�ʹ��Ԥ����ͷ�ļ�
	SET_TARGET_PROPERTIES(${PCH_TARGET} PROPERTIES COMPILE_FLAGS 
		"/Yu${PCH_HEADER_FILE} /FI${PCH_HEADER_FILE} /Fp${PCH_DIR}/${PCH_NAME}.pch")
 
	# Ԥ����Դ�ļ���stdafx.cpp���������ԣ�����Ԥ�����ļ�
	SET_SOURCE_FILES_PROPERTIES(${PCH_SOURCE_FILE} PROPERTIES COMPILE_FLAGS
		"/Yc${PCH_HEADER_FILE}")
    
	# ��Ԥ�����ļ��ĵ�����б�
	SET_DIRECTORY_PROPERTIES(PROPERTIES
		ADDITIONAL_MAKE_CLEAN_FILES ${PCH_DIR}/${PCH_NAME}.pch)
ENDIF(MSVC)
ENDMACRO(USE_MSVC_PCH)


##
##
##
MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
    SET_SOURCE_FILES_PROPERTIES(${Sources}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")  
    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
  ENDIF(MSVC)
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)