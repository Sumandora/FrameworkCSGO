cmake_minimum_required(VERSION 3.20)

file(STRINGS "ProjectName" CMAKE_PROJECT_NAME)

project(${CMAKE_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -march=native -mtune=native -Wno-return-type -ggdb --no-gnu-unique -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -fdata-sections -ffunction-sections -Wl,--gc-sections -Wall -s -march=native -mtune=native -Wno-return-type -fvisibility=hidden -fvisibility-inlines-hidden -fno-rtti --no-gnu-unique -D'IM_ASSERT(_EXPR)='")

option(MENUPREVIEW "Builds a executable which previews the menu" OFF)

set(CMAKE_SKIP_RPATH ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/Source/*.cpp")

if (MENUPREVIEW)
    list(REMOVE_ITEM SOURCE_FILES "${CMAKE_SOURCE_DIR}/Source/Main.cpp")
    add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
else ()
    list(REMOVE_ITEM SOURCE_FILES "${CMAKE_SOURCE_DIR}/Source/MenuPreview.cpp") # No need for this file if we aren't previewing the menu
    add_library(${CMAKE_PROJECT_NAME} SHARED ${SOURCE_FILES})
endif ()
target_link_libraries(${CMAKE_PROJECT_NAME} dl SDL2 GL)

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC PROJECT_NAME="${CMAKE_PROJECT_NAME}")
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Source)

# Setup SDL
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC /usr/include/SDL2)

# Setup ImGui
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Dependencies/imgui)
file(GLOB IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/*.cpp")
list(APPEND IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/backends/imgui_impl_sdl2.cpp")
list(APPEND IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/backends/imgui_impl_opengl3.cpp")

if (MENUPREVIEW)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC MENUPREVIEW)
    list(REMOVE_ITEM IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui_demo.cpp")
    target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Dependencies/imgui/backends)
    list(APPEND IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/examples/example_sdl2_opengl3/main.cpp")
endif ()
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    list(REMOVE_ITEM IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui_demo.cpp")
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC IMGUI_DISABLE_DEMO_WINDOWS)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC IMGUI_DISABLE_DEBUG_TOOLS)
endif ()
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${IMGUI_SOURCE_FILES})

# Setup xorstr
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Dependencies/xorstr/include)

# Setup mINI
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Dependencies/mINI/src)
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC MINI_CASE_SENSITIVE)

# Setup length-disassembler
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Dependencies/length-disassembler)
file(GLOB LDE_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/length-disassembler/*.cpp")
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${LDE_SOURCE_FILES})

# Setup DetourHooking
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Dependencies/DetourHooking/include)
file(GLOB DETOURHOOKING_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/DetourHooking/Source/*.cpp")
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${DETOURHOOKING_SOURCE_FILES})

# Setup ReturnAddressSpoofer
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Dependencies/ReturnAddressSpoofer/include)

# Setup SignatureScanner
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Dependencies/SignatureScanner/include)
file(GLOB SIGNATURESCANNER_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/SignatureScanner/Source/*.cpp")
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${SIGNATURESCANNER_SOURCE_FILES})

# Setup HideSharedObject
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Dependencies/HideSharedObject/include)
file(GLOB HIDESHAREDOBJECT_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/HideSharedObject/Source/*.cpp")
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${HIDESHAREDOBJECT_SOURCE_FILES})