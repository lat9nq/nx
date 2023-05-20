cmake_minimum_required(VERSION 3.20)

project(nx)

# libnx
set(LIBNX_INCLUDE_DIRS $ENV{DEVKITPRO}/libnx/include CACHE PATH "libnx include directory")
find_library(LIBNX_LIBRARY libnx.a PATHS $ENV{DEVKITPRO}/libnx/lib REQUIRED)
find_file(SWITCH_SPECS_FILE switch.specs PATHS $ENV{DEVKITPRO}/libnx REQUIRED)

# dkp aarch64
set(DEVKITPROA64_INCLUDE_DIRS $ENV{DEVKITPRO}/devkitA64/aarch64-none-elf/include CACHE PATH "devkitpro aarch64 include directory")
find_program(ELF2NRO_PROG elf2nro PATHS $ENV{DEVKITPRO}/tools/bin REQUIRED)

add_subdirectory(src)
