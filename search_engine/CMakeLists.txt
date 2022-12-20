# CMakeList.txt: проект CMake для search_engine; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

set (JSON_BuildTest OFF CACHE INTERNAL "")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (search_engine "main.cpp" "search_engine.h")

add_subdirectory (nlohmann_json)

# TODO: Добавьте тесты и целевые объекты, если это необходимо.

target_link_libraries (search_engine PRIVATE nlohmann_json::nlohmann_json)