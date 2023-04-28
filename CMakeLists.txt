cmake_minimum_required(VERSION 3.20)
project(FrameworkCSGO)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -march=native -mtune=native -Wno-return-type -ggdb --no-gnu-unique -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -fdata-sections -ffunction-sections -Wl,--gc-sections -Wall -s -march=native -mtune=native -Wno-return-type -fvisibility=hidden -fvisibility-inlines-hidden -fno-rtti --no-gnu-unique -D'IM_ASSERT(_EXPR)='")

set(CMAKE_SKIP_RPATH ON)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/Source/*.cpp")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(FrameworkCSGO SHARED ${SOURCE_FILES})
target_link_libraries(FrameworkCSGO dl SDL2 GL)

# Setup SDL
target_include_directories(FrameworkCSGO PUBLIC /usr/include/SDL2)

# Setup ImGui
target_include_directories(FrameworkCSGO PUBLIC Dependencies/imgui)
file(GLOB IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/*.cpp")
list(APPEND IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/backends/imgui_impl_sdl2.cpp")
list(APPEND IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/backends/imgui_impl_opengl3.cpp")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
	list(REMOVE_ITEM IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui_demo.cpp")
	target_compile_definitions(FrameworkCSGO PUBLIC IMGUI_DISABLE_DEMO_WINDOWS)
	target_compile_definitions(FrameworkCSGO PUBLIC IMGUI_DISABLE_DEBUG_TOOLS)
endif()
target_sources(FrameworkCSGO PUBLIC ${IMGUI_SOURCE_FILES})

# Setup xorstr
target_include_directories(FrameworkCSGO PUBLIC Dependencies/xorstr/include)

# Setup mINI
target_include_directories(FrameworkCSGO PUBLIC Dependencies/mINI/src)
target_compile_definitions(FrameworkCSGO PUBLIC MINI_CASE_SENSITIVE)

# Setup length-disassembler
target_include_directories(FrameworkCSGO PUBLIC Dependencies/length-disassembler)
file(GLOB LDE_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/length-disassembler/*.cpp")
target_sources(FrameworkCSGO PUBLIC ${LDE_SOURCE_FILES})

# Setup DetourHooking
target_include_directories(FrameworkCSGO PUBLIC Dependencies/DetourHooking/include)
file(GLOB DETOURHOOKING_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/DetourHooking/Source/*.cpp")
target_sources(FrameworkCSGO PUBLIC ${DETOURHOOKING_SOURCE_FILES})

# Setup ReturnAddressSpoofer
target_include_directories(FrameworkCSGO PUBLIC Dependencies/ReturnAddressSpoofer/include)

# Setup SignatureScanner
target_include_directories(FrameworkCSGO PUBLIC Dependencies/SignatureScanner/include)
file(GLOB SIGNATURESCANNER_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/SignatureScanner/Source/*.cpp")
target_sources(FrameworkCSGO PUBLIC ${SIGNATURESCANNER_SOURCE_FILES})