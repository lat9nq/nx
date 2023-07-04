add_executable(time_shared_mem_elf EXCLUDE_FROM_ALL
    main_shared_memory.c)

target_link_libraries(time_shared_mem_elf PRIVATE ${LIBNX_LIBRARY})


add_custom_target(time_shared_mem ALL 
    COMMAND
        ${ELF2NRO_PROG} time_shared_mem_elf.elf time_shared_mem.nro
    DEPENDS
        time_shared_mem_elf)