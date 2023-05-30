add_executable(minimal_elf EXCLUDE_FROM_ALL
    main.c)

target_link_libraries(minimal_elf PRIVATE ${LIBNX_LIBRARY})

add_custom_target(minimal ALL 
    COMMAND
        ${ELF2NRO_PROG} minimal_elf.elf minimal.nro
    DEPENDS
        minimal_elf)
