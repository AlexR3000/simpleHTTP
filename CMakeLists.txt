cmake_minimum_required(VERSION 3.10)


project(myproject)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the C++ standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Enable all warnings and treat warnings as errors.
# Warning on signed/unsigned comparison is disabled to allow comparison of
# size_t and int, which is common in C++ code.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wno-sign-compare)
    # Enable sanitizers in Debug builds
    if(CMAKE_BUILD_TYPE MATCHES "Debug")
        add_compile_options(-fsanitize=address -fsanitize=undefined)
        add_link_options(-fsanitize=address -fsanitize=undefined)
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4 /WX /wd4018)
endif()

# Clang-Tidy integration to enforce conformity to the C++ Core Guidelines
option(ENABLE_CLANG_TIDY "Enable clang-tidy analysis" ON)
if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()


# Create a library from the project files
# ADD ALL YOUR SOURCE FILES HERE
add_library(myproject  src/requestReceiver.cpp src/http/request.cpp src/http/requestParser.cpp)
target_include_directories(myproject PUBLIC "include")

# Create an executable from the main file, linked to the project library
add_executable(app src/main.cpp)
target_link_libraries(app myproject)


# Find GoogleTest package and create a test executable
find_package(GTest CONFIG REQUIRED)
enable_testing()
add_executable(tests  tests/http/requestParserTest.cpp) # ADD ALL YOUR TEST FILES HERE
target_link_libraries(tests PRIVATE myproject GTest::gtest GTest::gtest_main)
add_test(NAME Tests COMMAND tests)


# Custom target to create a zip file (including the directories include, src,
# tests and the CMakeLists.txt file)
add_custom_target(zip
    COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_BINARY_DIR}/project.zip" --format=zip
    -- "src/" "include/" "tests/" "CMakeLists.txt"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Creating a zip file of the project"
)

