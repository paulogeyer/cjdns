if (GCRYPT_INCLUDE_DIRS AND GCRYPT_LIBRARIES)
    set(GCRYPT_FOUND TRUE)
endif (GCRYPT_INCLUDE_DIRS AND GCRYPT_LIBRARIES)


if (NOT GCRYPT_FOUND)
    find_program(CONFIG_EXEC libgcrypt-config)
    if (CONFIG_EXEC)
        execute_process(COMMAND ${CONFIG_EXEC} "--cflags" OUTPUT_VARIABLE GCRYPT_INCLUDE_DIRS)
        execute_process(COMMAND ${CONFIG_EXEC} "--libs" OUTPUT_VARIABLE GCRYPT_LIBRARIES)
        if (GCRYPT_INCLUDE_DIRS AND GCRYPT_LIBRARIES)
            string(STRIP ${GCRYPT_INCLUDE_DIRS} GCRYPT_INCLUDE_DIRS)
            string(STRIP ${GCRYPT_LIBRARIES} GCRYPT_LIBRARIES)
            set(GCRYPT_FOUND TRUE)
        endif (GCRYPT_INCLUDE_DIRS AND GCRYPT_LIBRARIES)
    endif (CONFIG_EXEC)
endif (NOT GCRYPT_FOUND)


if (GCRYPT_FOUND)
    message(STATUS "Found libgcrypt: includes ${GCRYPT_INCLUDE_DIRS} libs ${GCRYPT_LIBRARIES}")
else (GCRYPT_FOUND)
    if (GCrypt_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find libgcrypt, try to setup GCRYPT_PREFIX accordingly")
    endif (GCrypt_FIND_REQUIRED)
endif (GCRYPT_FOUND)
