#-----------------------------------------------------------------------
#
# This is minimal game skeleton in HGE based on HGE_TUTORIAL 06
# to adapt this to your new game, search and replace HGESKEL with your project name
#
#-----------------------------------------------------------------------
project( HGESKEL )
cmake_minimum_required( VERSION 2.8 )


##--- set this to 8 or 9 ---
SET( HGE_DIRECTX_VER "9" )          # will be ignored by original HGE from relishgames.com
SET( HGE_UNICODE "0" )              # will be ignored by original HGE from relishgames.com
SET( HGE_INCLUDE_DIR "../hge181" )  # this will be used to find "include/" dir for "hge.h"



add_definitions( "-DHGE_DIRECTX_VER=${HGE_DIRECTX_VER}" "-DHGE_UNICODE=${HGE_UNICODE}" )
# disable warnings on STD non conformance and security issues
add_definitions( "-D_CRT_SECURE_NO_WARNINGS" "-D_CRT_NONSTDC_NO_WARNINGS" "-D_SCL_SECURE_NO_WARNINGS" "-D_WIN32_WINNT=0x0501" "/MP" )

#------ sources ------
set( HGESKEL_SRC
    src/main.cpp
    src/menuitem.cpp
    src/menuitem.h )

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY         ${HGESKEL_SOURCE_DIR}/bin CACHE PATH "Single Directory for all executables." )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${HGESKEL_SOURCE_DIR}/bin CACHE PATH "Single Directory for all executables." )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY         ${HGESKEL_SOURCE_DIR}/linklib CACHE PATH "Single Directory for all static libraries." )

#------ additional includes ------
include_directories( ${HGE_INCLUDE_DIR} ${HGE_INCLUDE_DIR}/include )

#------ link directories listed ------
link_directories( ${HGESKEL_SOURCE_DIR}/linklib )
link_libraries( hge hgehelpers )

#------ target -----
add_executable( HGESKEL WIN32 ${HGESKEL_SRC} )

if( MSVC )
    SET_TARGET_PROPERTIES( HGESKEL PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libci" )
endif()
