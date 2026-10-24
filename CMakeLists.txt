# dude-quake4-game: the Quake 4 game library (q4base.so / q4base.dll) built against DUDE.
#
# Normally configured by DUDE's tools/quake4/build-q4game.sh, which passes the DUDE checkout,
# its build directory and the compile flags DUDE uses for its own game libraries (read from
# that build), so this library is built exactly like base.so. By hand:
#   cmake -S . -B build -DDUDE_DIR=/path/to/dude -DDUDE_GAME_DEFINES="..." -DDUDE_GAME_FLAGS="..."
cmake_minimum_required(VERSION 3.16)
project(dude_quake4_game CXX)

set(DUDE_DIR "" CACHE PATH "the DUDE checkout whose headers this library is built against")
set(DUDE_BUILD_DIR "${DUDE_DIR}/build" CACHE PATH "DUDE's build directory (config.h, libidlib.a)")
set(DUDE_GAME_DEFINES "" CACHE STRING "preprocessor definitions DUDE builds its game libraries with")
set(DUDE_GAME_FLAGS "" CACHE STRING "compiler flags DUDE builds its game libraries with")
set(DUDE_GAME_INCLUDES "" CACHE STRING "system include directories DUDE's game libraries need (SDL, OpenAL)")
if(NOT DUDE_DIR OR NOT EXISTS "${DUDE_DIR}/neo/framework/Game.h")
	message(FATAL_ERROR "DUDE_DIR must point at a DUDE checkout (neo/framework/Game.h)")
endif()

# the single-player game: the sources of Raven's own project, source/game.vcproj
file(STRINGS "${CMAKE_SOURCE_DIR}/source/game.vcproj" vcproj_lines REGEX "RelativePath=\"[^\"]+\\.cpp\"")
set(q4game_sources "")
foreach(line ${vcproj_lines})
	string(REGEX REPLACE ".*RelativePath=\"([^\"]+)\".*" "\\1" path "${line}")
	string(REPLACE "\\" "/" path "${path}")
	string(REGEX REPLACE "^\\./" "" path "${path}")
	list(APPEND q4game_sources "${CMAKE_SOURCE_DIR}/source/${path}")
endforeach()
list(LENGTH q4game_sources q4game_count)
message(STATUS "Quake 4 game: ${q4game_count} sources from game.vcproj")

add_library(q4base SHARED ${q4game_sources})
set_target_properties(q4base PROPERTIES PREFIX "")
# DUDE's headers first, and never this tree's engine headers: every engine interface and every
# shared type must be DUDE's (source/idlib/precompiled.h). The game reaches its own headers by
# relative paths, as Raven wrote them.
target_include_directories(q4base PRIVATE "${DUDE_DIR}/neo" "${DUDE_BUILD_DIR}")
if(DUDE_GAME_INCLUDES)
	separate_arguments(dude_includes NATIVE_COMMAND "${DUDE_GAME_INCLUDES}")
	target_include_directories(q4base SYSTEM PRIVATE ${dude_includes})
endif()
separate_arguments(dude_defines NATIVE_COMMAND "${DUDE_GAME_DEFINES}")
separate_arguments(dude_flags NATIVE_COMMAND "${DUDE_GAME_FLAGS}")
target_compile_options(q4base PRIVATE ${dude_defines} ${dude_flags})
# Raven's SDK build defined Q4SDK (source/SConstruct); GAME_DLL comes with DUDE's definitions
target_compile_definitions(q4base PRIVATE Q4SDK)
target_link_libraries(q4base PRIVATE "${DUDE_BUILD_DIR}/libidlib.a")
