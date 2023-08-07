cmake_minimum_required(VERSION 3.20)

file(STRINGS "ProjectName" CMAKE_PROJECT_NAME)

project(${CMAKE_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_SKIP_RPATH ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_POLICY_DEFAULT_CMP0002 OLD)
cmake_policy(SET CMP0002 OLD)
set(ALLOW_DUPLICATE_CUSTOM_TARGETS TRUE)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/Source/*.cpp")

option(MENUPREVIEW "Builds an executable which previews the menu" OFF)
if (MENUPREVIEW)
    list(REMOVE_ITEM SOURCE_FILES "${CMAKE_SOURCE_DIR}/Source/Main.cpp")
    add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
else ()
    list(REMOVE_ITEM SOURCE_FILES "${CMAKE_SOURCE_DIR}/Source/MenuPreview.cpp") # No need for this file if we aren't previewing the menu
    add_library(${CMAKE_PROJECT_NAME} SHARED ${SOURCE_FILES})
endif ()
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE dl SDL2 GL)

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
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE Dependencies/SimpleJSON)

# Setup HideSharedObject
add_subdirectory("Dependencies/HideSharedObject")
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE HideSharedObject)

# Setup ReturnAddressSpoofer
add_subdirectory("Dependencies/ReturnAddressSpoofer")
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ReturnAddressSpoofer)
add_custom_command(
	TARGET ${CMAKE_PROJECT_NAME} PRE_LINK
	COMMAND "$<TARGET_FILE:ObjectFileRewriter>" "${CMAKE_BINARY_DIR}/CMakeFiles/${CMAKE_PROJECT_NAME}.dir"
	DEPENDS ObjectFileRewriter
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE SIGNATURESCANNER_ENABLE_IDA_SEARCH)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE SIGNATURESCANNER_ENABLE_STRING_SEARCH)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE SIGNATURESCANNER_ENABLE_XREF_SEARCH)

# Setup bcrl
add_subdirectory("Dependencies/bcrl")
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE bcrl)

# Setup length-disassembler
if (NOT (TARGET length-disassembler))
    add_subdirectory("Dependencies/length-disassembler")
endif ()
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE length-disassembler)

# Setup DetourHooking
if (NOT (TARGET DetourHooking))
    add_subdirectory("Dependencies/DetourHooking")
endif ()
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE DetourHooking)

# Setup SignatureScanner
if (NOT (TARGET SignatureScanner))
    add_subdirectory("Dependencies/SignatureScanner")
endif ()
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SignatureScanner)
