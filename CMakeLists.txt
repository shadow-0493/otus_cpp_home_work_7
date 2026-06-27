cmake_minimum_required(VERSION 3.15)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(bulk VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bulk src/main.cpp
               src/parser/command_parser.cpp)

if (MSVC)
    target_compile_options(bulk PRIVATE /W4)
else ()
    target_compile_options(bulk PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS bulk RUNTIME DESTINATION bin)

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

