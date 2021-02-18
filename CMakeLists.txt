#-----------------------------------------------------------------------
#
# This is minimal game skeleton in HGE based on HGE_TUTORIAL 06
# to adapt this to your new game, search and replace HGESKEL with your project name
#
#  This program is free software. It comes without any warranty, to
#  the extent permitted by applicable law. You can redistribute it
#  and/or modify it under the terms of the Do What The Fuck You Want
#  To Public License, Version 2, as published by Sam Hocevar. See
#  http://sam.zoy.org/wtfpl/COPYING for more details.
#
#-----------------------------------------------------------------------
PROJECT(HGESKEL)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

##--- set this to 8 or 9 ---
SET(HGE_UNICODE "0")              # will be ignored by original HGE from relishgames.com

# Path to HGE source downloaded from github.com/kvakvs/hge
# this will be used to find "include/" dir for "hge.h"
SET(HGE_SOURCE_DIR "../HGE")

GET_FILENAME_COMPONENT(HGE_SOURCE_DIR ${HGE_SOURCE_DIR} ABSOLUTE)

ADD_DEFINITIONS("-DHGE_DIRECTX_VER=${HGE_DIRECTX_VER}" "-DHGE_UNICODE=${HGE_UNICODE}")
# disable warnings on STD non conformance and security issues
ADD_DEFINITIONS("-D_CRT_SECURE_NO_WARNINGS"
        "-D_CRT_NONSTDC_NO_WARNINGS"
        "-D_SCL_SECURE_NO_WARNINGS"
        "-D_WIN32_WINNT=0x0501" "/MP")

#------ sources ------
SET(HGESKEL_SRC
        src/game.cpp src/game.h
        src/game_state.cpp src/game_state.h
        src/menuitem.cpp src/menuitem.h
        src/player.cpp src/player.h
        src/world.cpp src/world.h
        src/creature.cpp src/creature.h
        src/projectile.cpp src/projectile.h
        src/sprite_manager.cpp src/sprite_manager.h
        )

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${HGESKEL_SOURCE_DIR}/bin CACHE PATH "Single Directory for all executables.")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${HGESKEL_SOURCE_DIR}/bin CACHE PATH "Single Directory for all executables.")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${HGESKEL_SOURCE_DIR}/linklib CACHE PATH "Single Directory for all static libraries.")

#------ additional includes ------
INCLUDE_DIRECTORIES(${HGE_SOURCE_DIR} ${HGE_SOURCE_DIR}/include)

#------ link directories listed ------
LINK_DIRECTORIES(${HGE_SOURCE_DIR}/linklib
        ${HGE_SOURCE_DIR}/linklib/Debug
        ${HGE_SOURCE_DIR}/linklib/Release)
LINK_LIBRARIES(hge hgehelpers)

#------ target -----
ADD_EXECUTABLE(HGESKEL WIN32 ${HGESKEL_SRC})

if (MSVC)
    SET_TARGET_PROPERTIES(HGESKEL PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libci")
endif ()
