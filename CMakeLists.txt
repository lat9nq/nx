cmake_minimum_required(VERSION 3.20)

project(nx)

# libnx
set(LIBNX_INCLUDE_DIRS $ENV{DEVKITPRO}/libnx/include CACHE PATH "libnx include directory")
find_library(LIBNX_LIBRARY libnx.a PATHS $ENV{DEVKITPRO}/libnx/lib REQUIRED)
find_file(SWITCH_SPECS_FILE switch.specs PATHS $ENV{DEVKITPRO}/libnx REQUIRED)

# dkp aarch64
set(DEVKITPROA64_INCLUDE_DIRS $ENV{DEVKITPRO}/devkitA64/aarch64-none-elf/include CACHE PATH "devkitpro aarch64 include directory")
find_program(ELF2NRO_PROG elf2nro PATHS $ENV{DEVKITPRO}/tools/bin REQUIRED)

include_directories(SYSTEM "${LIBNX_INCLUDE_DIRS}")

add_link_options(
    -specs=${SWITCH_SPECS_FILE}
    -march=armv8-a+crc+crypto
    -mtune=cortex-a57
    -mtp=soft
    -fPIE)

add_compile_options(
    -ffunction-sections
    -march=armv8-a+crc+crypto
    -mtune=cortex-a57
    -fPIE
    
    -Werror=all
    -Werror=extra

# Not sure why this doesn't work with include_directories(), so do it here
    -isystem "${DEVKITPROA64_INCLUDE_DIRS}"
    )

add_compile_definitions(SWITCH)

add_subdirectory(time)
