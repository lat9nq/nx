include_directories(.)
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
add_subdirectory(minimal)
