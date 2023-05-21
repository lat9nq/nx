add_executable(time_elf EXCLUDE_FROM_ALL
    main.c
    time_zone.h
    time_zone.c)

target_link_libraries(time_elf PRIVATE ${LIBNX_LIBRARY})

add_custom_target(time ALL 
    COMMAND
        ${ELF2NRO_PROG} time_elf.elf time.nro
    DEPENDS
        time_elf)
