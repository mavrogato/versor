
cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER icx)
set(CMAKE_C_FLAGS "-Wall -Wextra -std=c17")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_COMPILER icpx)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++2b -sycl-std=2020")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(versor)
find_package(IntelDPCPP REQUIRED)

#############################################################################
# https://github.com/Cloudef/wlc/blob/master/CMake/FindWaylandProtocols.cmake
include(FeatureSummary)
set_package_properties(WaylandProtocols PROPERTIES
  URL "https://cgit.freedesktop.org/wayland/wayland-protocols"
  DESCRIPTION "Wayland protocol development")

unset(WAYLANDPROTOCOLS_PATH)
find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
  execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=pkgdatadir wayland-protocols
    OUTPUT_VARIABLE WAYLANDPROTOCOLS_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(WaylandProtocols DEFAULT_MSG WAYLANDPROTOCOLS_PATH)
mark_as_advanced(WAYLANDPROTOCOLS_PATH)

set(XDG_SHELL_V6_PROTOCOL ${WAYLANDPROTOCOLS_PATH}/unstable/xdg-shell/xdg-shell-unstable-v6.xml)
set(ZWP_TABLET_V2_PROTOCOL ${WAYLANDPROTOCOLS_PATH}/unstable/tablet/tablet-unstable-v2.xml)
add_custom_command(
  OUTPUT xdg-shell-v6-private.c
  COMMAND wayland-scanner client-header ${XDG_SHELL_V6_PROTOCOL} xdg-shell-v6-client.h
  COMMAND wayland-scanner private-code  ${XDG_SHELL_V6_PROTOCOL} xdg-shell-v6-private.c)
add_custom_command(
  OUTPUT zwp-tablet-v2-private.c
  COMMAND wayland-scanner client-header ${ZWP_TABLET_V2_PROTOCOL} zwp-tablet-v2-client.h
  COMMAND wayland-scanner private-code  ${ZWP_TABLET_V2_PROTOCOL} zwp-tablet-v2-private.c)

#############################################################################
file(GLOB HDR *.hpp *.h)
file(GLOB SRC *.cc *.c)
list(FILTER SRC EXCLUDE REGEX ".*test\.cc")
file(GLOB TST *test.cc)

add_executable(versor
  ${HDR}
  ${SRC}
  ${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-v6-private.c
  ${CMAKE_CURRENT_BINARY_DIR}/zwp-tablet-v2-private.c)

target_link_libraries(versor
  wayland-client)

#############################################################################
# https://github.com/NVIDIA/cutlass/blob/master/cmake/googletest.cmake
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.13.0)
FetchContent_MakeAvailable(googletest)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  if (MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  endif()
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

enable_testing()

add_executable(versor-test
  ${HDR}
  ${TST})
target_link_libraries(versor-test
  PRIVATE
  gtest_main)
include(GoogleTest)
gtest_discover_tests(versor-test)

#############################################################################
add_custom_target(run
  DEPENDS versor versor-test
  COMMAND ./versor-test && ./versor)
