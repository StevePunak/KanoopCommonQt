find_package(Qt6 REQUIRED COMPONENTS Test)

function(add_kanoop_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE
        Qt6::Test
        Qt6::Network
        Qt6::Xml
        KanoopCommonQt
    )
    target_include_directories(${name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_kanoop_test(tst_concurrency)
add_kanoop_test(tst_daterange)
add_kanoop_test(tst_string)
add_kanoop_test(tst_geo)
add_kanoop_test(tst_timespan)
add_kanoop_test(tst_geometry)
add_kanoop_test(tst_misc)
