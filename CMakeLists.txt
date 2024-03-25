cmake_minimum_required(VERSION 3.16)
include(GNUInstallDirs)

project(KanoopCommon)

find_package(Qt6 REQUIRED COMPONENTS Network)
qt_standard_project_setup()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(KANOOP_COMMON_VERSION "1.1.3")

file(GLOB_RECURSE KANOOP_COMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE KANOOP_COMMON_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

qt_add_library(KanoopCommon ${KANOOP_COMMON_SOURCES} ${KANOOP_COMMON_HEADERS})

set_target_properties(KanoopCommon PROPERTIES VERSION "${KANOOP_COMMON_VERSION}")
set_target_properties(KanoopCommon PROPERTIES SOVERSION 1)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/libKanoopCommon.pc.in" "libKanoopCommon.pc" @ONLY)

target_include_directories(KanoopCommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(KanoopCommon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(KanoopCommon PRIVATE Qt6::Network)

add_compile_definitions(KANOOP_LIBRARY)
add_compile_definitions(QT_DEPRECATED_WARNINGS)
add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x060000)  # Disables all the APIs deprecated before Qt 6.0.0

# Set install prefix.  This is necessary when installing without KYBER_OUTPUT_PREFIX set as this causes the path to start with two
# forward-slashes which CMake interprets to mean a network path.  Thus, we only include the "/" if there is something ahead of it.
if (DEFINED $ENV{KYBER_OUTPUT_PREFIX})
    set(KANOOP_INSTALL_PREFIX "$ENV{KYBER_OUTPUT_PREFIX}/")
endif()

install(TARGETS KanoopCommon LIBRARY DESTINATION "${KANOOP_INSTALL_PREFIX}${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/Kanoop" DESTINATION "${KANOOP_INSTALL_PREFIX}${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}" FILES_MATCHING PATTERN "*.h")
