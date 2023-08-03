cmake_minimum_required(VERSION 3.20)

file(STRINGS "ProjectName" CMAKE_PROJECT_NAME)

project(${CMAKE_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_SKIP_RPATH ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/Source/*.cpp")

option(MENUPREVIEW "Builds an executable which previews the menu" OFF)
if (MENUPREVIEW)
    list(REMOVE_ITEM SOURCE_FILES "${CMAKE_SOURCE_DIR}/Source/Main.cpp")
    add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
else ()
    list(REMOVE_ITEM SOURCE_FILES "${CMAKE_SOURCE_DIR}/Source/MenuPreview.cpp") # No need for this file if we aren't previewing the menu
    add_library(${CMAKE_PROJECT_NAME} SHARED ${SOURCE_FILES})
endif ()
target_link_libraries(${CMAKE_PROJECT_NAME} dl SDL2 GL)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE "-fno-rtti" "-ffunction-sections" "-march=native")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE "-Wall" "-ggdb" "-DDEBUG")
else()
	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE "-Os" "-fvisibility=hidden" "-fvisibility-inlines-hidden")
	set(CMAKE_CXX_FLAGS "-D'IM_ASSERT(_EXPR)=((void)(_EXPR))' ${CMAKE_CXX_FLAGS}") # TODO Can you do this without set?
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE "-fimplicit-constexpr" "--no-gnu-unique")
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
		target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE "-fdata-sections" "-s")
		target_link_options(${CMAKE_PROJECT_NAME} PRIVATE "-Wl,--gc-sections")
    endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE "-Wno-unknown-attributes" "-Wno-unused-private-field" "-stdlib=libc++")
endif ()

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE PROJECT_NAME="${CMAKE_PROJECT_NAME}")

# Setup SDL
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE /usr/include/SDL2)

# Setup ImGui
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE Dependencies/imgui)
file(GLOB IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/*.cpp")
list(APPEND IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/backends/imgui_impl_sdl2.cpp")
list(APPEND IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/backends/imgui_impl_opengl3.cpp")
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC IMGUI_DEFINE_MATH_OPERATORS) # TODO are there more improvements that can be made with this?

if (MENUPREVIEW)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC MENUPREVIEW)
    list(REMOVE_ITEM IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui_demo.cpp")
    target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Dependencies/imgui/backends)
    list(APPEND IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/examples/example_sdl2_opengl3/main.cpp")
endif ()
if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(REMOVE_ITEM IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui_demo.cpp")
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC IMGUI_DISABLE_DEMO_WINDOWS)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC IMGUI_DISABLE_DEBUG_TOOLS)
endif ()
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${IMGUI_SOURCE_FILES})

# Setup SimpleJSON
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Dependencies/SimpleJSON)

# Setup length-disassembler
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Dependencies/length-disassembler)
file(GLOB LDE_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/length-disassembler/*.cpp")
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${LDE_SOURCE_FILES})

# Setup DetourHooking
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Dependencies/DetourHooking/Include)
file(GLOB DETOURHOOKING_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/DetourHooking/Source/*.cpp")
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${DETOURHOOKING_SOURCE_FILES})

# Setup ReturnAddressSpoofer
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "Dependencies/ReturnAddressSpoofer/Include")
add_subdirectory("Dependencies/ReturnAddressSpoofer/ObjectFileRewriter")
add_custom_command(
	TARGET ${CMAKE_PROJECT_NAME} PRE_LINK
	COMMAND "$<TARGET_FILE:ObjectFileRewriter>" "${CMAKE_BINARY_DIR}/CMakeFiles/${CMAKE_PROJECT_NAME}.dir"
	DEPENDS ObjectFileRewriter
)

# Setup SignatureScanner
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Dependencies/SignatureScanner/include)
file(GLOB_RECURSE SIGNATURESCANNER_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/SignatureScanner/Source/*.cpp")
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${SIGNATURESCANNER_SOURCE_FILES})

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE SIGNATURESCANNER_ENABLE_IDA_SEARCH)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE SIGNATURESCANNER_ENABLE_STRING_SEARCH)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE SIGNATURESCANNER_ENABLE_XREF_SEARCH)

# Setup HideSharedObject
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Dependencies/HideSharedObject/include)
file(GLOB HIDESHAREDOBJECT_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/HideSharedObject/Source/*.cpp")
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${HIDESHAREDOBJECT_SOURCE_FILES})

# Setup bcrl
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC Dependencies/bcrl/Include)
file(GLOB_RECURSE BCRL_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Dependencies/bcrl/Source/*.cpp")
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${BCRL_SOURCE_FILES})
