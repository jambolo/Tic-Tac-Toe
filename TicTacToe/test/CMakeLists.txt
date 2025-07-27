cmake_minimum_required(VERSION 3.21)

find_package(GTest REQUIRED)
include(GoogleTest)

# Function to create test executables
function(add_test test_name source_file)
    add_executable(${test_name} ${source_file})
    set_target_properties(${test_name} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
    if(WIN32)
        target_compile_definitions(${test_name}
            PRIVATE
                NOMINMAX
                WIN32_LEAN_AND_MEAN
                VC_EXTRALEAN
                _CRT_SECURE_NO_WARNINGS
                _SECURE_SCL=0
                _SCL_SECURE_NO_WARNINGS
        )
    endif()

    target_link_libraries(${test_name} 
        PRIVATE 
            TicTacToe::TicTacToe
            GTest::gtest
            GTest::gtest_main
    )
    gtest_discover_tests(${test_name})
    message(STATUS "Added test executable: ${test_name}")
endfunction()

set(SOURCES
    test-Board.cpp
    test-ComputerPlayer.cpp
    test-HumanPlayer.cpp
    test-TicTacToeEvaluator.cpp
    test-TicTacToeState.cpp
    test-ZHash.cpp
)

message(STATUS "Building tests for TicTacToe")

foreach(FILE ${SOURCES})
    get_filename_component(TEST ${FILE} NAME_WE)
    add_test("TicTacToe_${TEST}" ${FILE})
endforeach()
