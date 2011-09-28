#-----------------------------------------------------------------------
#
# This is minimal game skeleton in HGE based on HGE_TUTORIAL 06
# to adapt this to your new game, search and replace HGESKEL with your project name
#
#-----------------------------------------------------------------------
project( HGESKEL )
cmake_minimum_required( VERSION 2.8 )


##--- set this to 8 or 9 ---
SET( HGE_DIRECTX_VER "9" )
SET( HGE_UNICODE "0" )

add_definitions( "-DHGE_DIRECTX_VER=${HGE_DIRECTX_VER}" "-DHGE_UNICODE=${HGE_UNICODE}" )
# disable warnings on STD non conformance and security issues
add_definitions( "-D_CRT_SECURE_NO_WARNINGS" "-D_CRT_NONSTDC_NO_WARNINGS" "-D_SCL_SECURE_NO_WARNINGS" "-D_WIN32_WINNT=0x0501" "/MP" )

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${HGE_SOURCE_DIR}/bin CACHE PATH "Single Directory for all executables." )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${HGE_SOURCE_DIR}/bin CACHE PATH "Single Directory for all executables." )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${HGE_SOURCE_DIR}/linklib CACHE PATH "Single Directory for all static libraries." )
SET( CMAKE_INCLUDE_CURRENT_DIR ON )

#------ sources ------
set( TUT06_SRC
    src/main.cpp
    src/menuitem.cpp
    src/menuitem.h )

#------ additional includes ------
include_directories( ${HGE_SOURCE_DIR}/include )
include_directories( ${HGE_SOURCE_DIR}/src/libPNG )
include_directories( ${HGE_SOURCE_DIR}/src/zlib-1.2.5 )
include_directories( ${DIRECTX_SDK_DIR}/include )

#------ link directories listed ------
link_directories( ${HGE_SOURCE_DIR}/linklib )
link_directories( ${DIRECTX_SDK_DIR}/lib ${DIRECTX_SDK_DIR}/lib/x64 ${DIRECTX_SDK_DIR}/lib/x86 )
link_libraries( hge )

#------ target -----
add_executable( HGESKEL WIN32 ${TUT06_SRC} )

if( MSVC )
    SET_TARGET_PROPERTIES( HGESKEL PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libci" )
endif()
