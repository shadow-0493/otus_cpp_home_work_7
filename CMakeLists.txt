cmake_minimum_required(VERSION 3.15)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(bulk VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bulk 
    src/main.cpp
    src/parser/command_parser.cpp
)

target_include_directories(bulk PRIVATE
    src/parser
    src/printer
)

install(TARGETS bulk RUNTIME DESTINATION bin)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE 
)
FetchContent_MakeAvailable(googletest)

add_executable(test_bulk
    tests/command_parser_tests.cpp
    src/parser/command_parser.cpp
)

target_include_directories(test_bulk PRIVATE
    src/parser
    src/printer
    tests
)

target_link_libraries(test_bulk
    GTest::gtest
    GTest::gtest_main
)

if (MSVC)
    target_compile_options(bulk PRIVATE /W4)
    target_compile_options(test_bulk PRIVATE /W4)
else ()
    target_compile_options(bulk PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(test_bulk PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME "bulk")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT "j.morzhov@reglab.ru")
set(CPACK_PACKAGE_DESCRIPTION "Command Parser (Homework 7)")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${PROJECT_VERSION}-Linux")

include(CPack)

enable_testing()
include(GoogleTest)
gtest_discover_tests(test_bulk)