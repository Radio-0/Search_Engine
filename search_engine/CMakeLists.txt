# CMakeList.txt: проект CMake для search_engine; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

set (JSON_BuildTest OFF CACHE INTERNAL "")

# Добавьте источник в исполняемый файл этого проекта.

#GTest
include (FetchContent)
FetchContent_Declare(googletest URL
	https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
#set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable (search_engine "main.cpp" "search_engine.h")

add_subdirectory (nlohmann_json)

# TODO: Добавьте тесты и целевые объекты, если это необходимо.

target_link_libraries (search_engine PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(search_engine PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(search_engine)