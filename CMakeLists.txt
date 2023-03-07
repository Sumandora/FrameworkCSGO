cmake_minimum_required(VERSION 3.20)
project(Framework_Example)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -march=native -mtune=native -Wno-return-type -ggdb -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Oz -fdata-sections -ffunction-sections -Wl,--gc-sections -Wall -s -march=native -mtune=native -Wno-return-type -fvisibility=hidden -fvisibility-inlines-hidden")

set(CMAKE_SKIP_RPATH ON)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/Source/*.cpp")

add_library(Framework_Example SHARED ${SOURCE_FILES})
target_link_libraries(Framework_Example dl SDL2 GL)

# Setup Framework
target_include_directories(Framework_Example PUBLIC Dependencies/Framework/Source/Framework)

file(GLOB_RECURSE FRAMEWORK_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/Framework/Source/Framework/*.cpp")
target_sources(Framework_Example PUBLIC ${FRAMEWORK_SOURCE_FILES})

target_compile_definitions(Framework_Example PUBLIC FRAMEWORK_ENABLE_HOOKING_DETOUR)
target_compile_definitions(Framework_Example PUBLIC FRAMEWORK_ENABLE_HOOKING_PTRSWAP)
target_compile_definitions(Framework_Example PUBLIC FRAMEWORK_ENABLE_PATTERN_SCANNING)
target_compile_definitions(Framework_Example PUBLIC FRAMEWORK_ENABLE_RETURN_ADDRESS)

# Setup ImGui
target_include_directories(Framework_Example PUBLIC Dependencies/imgui)
# SDL
target_include_directories(Framework_Example PUBLIC /usr/include/SDL2)

file(GLOB IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/*.cpp")
list(REMOVE_ITEM IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui_demo.cpp")

list(APPEND IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/backends/imgui_impl_sdl2.cpp")
list(APPEND IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/backends/imgui_impl_opengl3.cpp")
target_sources(Framework_Example PUBLIC ${IMGUI_SOURCE_FILES})

# Setup xorstr
target_include_directories(Framework_Example PUBLIC Dependencies/xorstr/include)

file(GLOB_RECURSE XORSTR_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/xorstr/include")
target_sources(Framework_Example PUBLIC ${XORSTR_SOURCE_FILES})
