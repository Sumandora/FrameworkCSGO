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

function(apply_obfuscation_parameters TARGET)
	target_compile_options(${TARGET} PRIVATE "-fno-rtti") # Force a compile time error in case the target is using any RTTI features
	if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
		target_compile_options(${TARGET} PRIVATE "-fvisibility=hidden" "-fvisibility-inlines-hidden")
	endif ()
endfunction()

apply_obfuscation_parameters(${CMAKE_PROJECT_NAME})

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE "-march=native")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE "-Wall" "-ggdb" "-DDEBUG")
else()
	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE "-Os")
	set(CMAKE_CXX_FLAGS "-D'IM_ASSERT(_EXPR)=((void)(_EXPR))' ${CMAKE_CXX_FLAGS}") # TODO Can you do this without set?
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE "--no-gnu-unique")
	if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 12)
		target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE "-fimplicit-constexpr")
	endif()
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

include(FetchContent)

FetchContent_Declare(bcrl
	GIT_REPOSITORY https://github.com/Sumandora/bcrl
	GIT_PROGRESS TRUE
	GIT_TAG f826973)
FetchContent_MakeAvailable(bcrl)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE bcrl)

# Setup DetourHooking
FetchContent_Declare(DetourHooking
	GIT_REPOSITORY https://github.com/Sumandora/DetourHooking
	GIT_PROGRESS TRUE
	GIT_TAG 4f14fde)
FetchContent_MakeAvailable(DetourHooking)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE DetourHooking)

# Setup ReturnAddressSpoofer
FetchContent_Declare(ReturnAddressSpoofer
	GIT_REPOSITORY https://github.com/Sumandora/ReturnAddressSpoofer
	GIT_PROGRESS TRUE
	GIT_TAG 04a655d)
FetchContent_MakeAvailable(ReturnAddressSpoofer)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ReturnAddressSpoofer)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE "-ffunction-sections")

add_subdirectory("${ReturnAddressSpoofer_SOURCE_DIR}/ObjectFileRewriter")
add_custom_command(
	TARGET ${CMAKE_PROJECT_NAME} PRE_LINK
	COMMAND "$<TARGET_FILE:ObjectFileRewriter>" "${CMAKE_BINARY_DIR}/CMakeFiles/${CMAKE_PROJECT_NAME}.dir"
	DEPENDS ObjectFileRewriter
)

# Setup HideSharedObject
FetchContent_Declare(HideSharedObject
	GIT_REPOSITORY https://github.com/Sumandora/HideSharedObject
	GIT_PROGRESS TRUE
	GIT_TAG b83ac20)
FetchContent_MakeAvailable(HideSharedObject)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE HideSharedObject)


function(apply_obfuscation_recursively TARGET)
	get_target_property(DEPENDENCIES ${TARGET} LINK_LIBRARIES)
	if(NOT DEPENDENCIES)
		return()
	endif()

	foreach(DEPENDENCY ${DEPENDENCIES})
		get_target_property(LIB_TYPE ${DEPENDENCY} TYPE)
		if(NOT LIB_TYPE STREQUAL "INTERFACE_LIBRARY")
			apply_obfuscation_parameters(${DEPENDENCY})
		endif ()
		apply_obfuscation_recursively(${DEPENDENCY})
	endforeach()

endfunction()

apply_obfuscation_recursively(${CMAKE_PROJECT_NAME})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE dl SDL2 GL)
