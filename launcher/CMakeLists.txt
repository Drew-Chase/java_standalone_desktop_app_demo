cmake_minimum_required(VERSION 4.2)
project(launcher C)

set(CMAKE_C_STANDARD 23)

add_executable(launcher main.c)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_PROJECT_NAME}")
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES INTERMEDIATE_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/obj/${CMAKE_PROJECT_NAME}")
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "-debug")
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME "${CMAKE_PROJECT_NAME}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(CMAKE_CACHEFILE_DIR ${PROJECT_SOURCE_DIR}/bin/obj/cmake/${CMAKE_PROJECT_NAME})

if (WIN32)
    # Build as a GUI subsystem app so no console window is allocated.
    # NOTE: with single-config generators (Ninja), this is decided at CONFIGURE
    # time, so a stale CMake cache configured without it will still produce a
    # console exe — wipe the build dir if you flip this.
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif ()

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Oz)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static -lpthread")

