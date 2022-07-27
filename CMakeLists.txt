cmake_minimum_required(VERSION 3.20)
project(Framework_Example)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(DEBUG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mtune=native -fpie -Wno-return-type")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -ftree-vectorize -march=native -mtune=native -fomit-frame-pointer -fpie -Wno-return-type -fvisibility=hidden")
endif()

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
