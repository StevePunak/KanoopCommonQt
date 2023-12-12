cmake_minimum_required(VERSION 3.16)
include(GNUInstallDirs)

project(KanoopCommon)

find_package(Qt6 REQUIRED COMPONENTS Network)
qt_standard_project_setup()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(KANOOP_COMMON_VERSION "1.0.21")

file(GLOB_RECURSE KANOOP_COMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE KANOOP_COMMON_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

qt_add_library(KanoopCommon ${KANOOP_COMMON_SOURCES} ${KANOOP_COMMON_HEADERS})

set_target_properties(KanoopCommon PROPERTIES OUTPUT_NAME "KanoopCommon" SUFFIX ".so.${KANOOP_COMMON_VERSION}")
set_target_properties(KanoopCommon PROPERTIES SOVERSION 1)
set_target_properties(KanoopCommon PROPERTIES PUBLIC_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/Kanoop/kanoopcommon.h")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/libKanoopCommon.pc.in" "libKanoopCommon.pc" @ONLY)

target_include_directories(KanoopCommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(KanoopCommon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(KanoopCommon PRIVATE Qt6::Network)

add_compile_definitions(KANOOP_LIBRARY)
add_compile_definitions(QT_DEPRECATED_WARNINGS)
add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x060000)  # Disables all the APIs deprecated before Qt 6.0.0

install(TARGETS KanoopCommon LIBRARY       DESTINATION "$ENV{OUTPUT_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
                             PUBLIC_HEADER DESTINATION "$ENV{OUTPUT_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/Kanoop")
