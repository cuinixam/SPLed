# Set system and processor
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-s3)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set compiler paths for normal and safety
set(IAR_COMP_DIR_NORMAL "$ENV{iar_ew_for_RL78_9p1p9_ROOT}/rl78/bin")
set(IAR_COMP_DIR_SAFETY "$ENV{iar_ew_for_RL78_9p1p9_FS_5p10p5_ROOT}/rl78/bin")

if(IAR_TARGET_PLATFORM_SAFETY)
    set(IAR_COMP_DIR "${IAR_COMP_DIR_SAFETY}")
else()
    set(IAR_COMP_DIR "${IAR_COMP_DIR_NORMAL}")
endif()

set(OUTPUT_FILE_EXTENSSION "")

set(COMPILER_ARGUMENT_STRING "--")

set(CMAKE_C_COMPILER ${IAR_COMP_DIR}/iccrl78.exe CACHE INTERNAL "")
set(CMAKE_CXX_COMPILER ${IAR_COMP_DIR}/iccrl78.exe CACHE INTERNAL "")
set(CMAKE_LINKER ${IAR_COMP_DIR}/ilinkrl78.exe CACHE INTERNAL "")
set(CMAKE_ASM_COMPILER ${IAR_COMP_DIR}/iasmrl78.exe CACHE INTERNAL "")

set(C_FLAGS_COMMON "--calling_convention v2 --double=32 --core s3 --code_model far --near_const_location rom0 --dlib_config normal -e --data_model far -D KEY_BIT_LEN_2048 -D OS_LIBMODE_SAFE=1 --no_cse --no_unroll --no_inline --no_code_motion --no_tbaa --no_cross_call --no_scheduling --no_clustering --source_encoding locale --remarks")
set(CXX_FLAGS_COMMON "")
set(ASM_FLAGS_COMMON "-D__FAR_MODEL__ -D__FAR_DATA_MODEL__ --remarks --code_model far --data_model far --core s3")
set(LINKER_FLAGS_COMMON "${PROJECT_SOURCE_DIR}/ThirdParty/lib/embOS/osRL78ff02_SAFE.a --config_def __RESERVE_OCD_ROM=1 --config_def __RESERVE_OCD_TRACE_RAM=0 ")

set(CMAKE_C_FLAGS "${C_FLAGS_COMMON}")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS_COMMON}")
set(CMAKE_ASM_FLAGS "${ASM_FLAGS_COMMON}")
set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS_COMMON}")

set(CMAKE_C_FLAGS_DEBUG "-DDEBUG --debug -On --no_cse --no_unroll --no_inline --no_code_motion --no_tbaa --no_clustering --no_scheduling" CACHE INTERNAL "")
set(CMAKE_ASM_FLAGS_DEBUG "-r " CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG -Oh" CACHE INTERNAL "")
set(CMAKE_ASM_FLAGS_RELEASE "-DNDEBUG " CACHE INTERNAL "")
set(CMAKE_C_FLAGS_MINSIZEREL "-DNDEBUG -Ohz" CACHE INTERNAL "")
set(CMAKE_ASM_FLAGS_MINSIZEREL "-DNDEBUG " CACHE INTERNAL "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

function(compiler_iar_gen_hex_file target_name)
    if(IAR_TARGET_PLATFORM_SAFETY)
        add_custom_command(TARGET ${LINK_TARGET_NAME}
            POST_BUILD
            COMMAND "${IAR_COMP_DIR}/ielftool.exe" --ihex ${LINKER_OUTPUT_FILE}.elf ${VARIANT}.hex
            COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} cmd /c "${CMAKE_SOURCE_DIR}/checksum_calculation_script_auto_rl78f24.bat" "${target_name}" "${CMAKE_SOURCE_DIR}/build" "${LINKER_OUTPUT_FILE}.elf"
            DEPENDS ${LINKER_OUTPUT_FILE}.elf
        )
    else()
        add_custom_command(TARGET ${LINK_TARGET_NAME}
            POST_BUILD
            COMMAND "${IAR_COMP_DIR}/ielftool.exe" --ihex ${LINKER_OUTPUT_FILE} ${VARIANT}.hex
            DEPENDS ${LINKER_OUTPUT_FILE}
            BYPRODUCTS ${VARIANT}.hex
        )
    endif()
endfunction(compiler_iar_gen_hex_file)

function(compiler_iar_set_target_properties entry_function keep_symbol_list linker_file)
    if(IAR_TARGET_PLATFORM_SAFETY)
        set_target_properties(${LINK_TARGET_NAME} PROPERTIES
            COMPILE_FLAGS "${target_compile_flags}"
            LINK_FLAGS " --map ${target_name}.map \
--config ${linker_file} \
--config_def _NEAR_HEAP_SIZE=0 \
--config_def _FAR_HEAP_SIZE=0 \
--config_def _HUGE_HEAP_SIZE=0 \
--config_def _STACK_SIZE=1280 \
--config_def _NEAR_CONST_LOCATION_SIZE=0x4F00 \
--config_def _NEAR_CONST_LOCATION_START=0x5000 \
--define_symbol _NEAR_CONST_LOCATION=0 \
--entry __iar_program_start \
--text_out locale \
--no_free_heap \
--keep __sector_crc_checksum \
--keep __hspeed_crc_checksum \
--keep __dflash_crc_checksum \
--keep _stack_g \
--place_holder __sector_crc_checksum,0xF0,.sector_crc_checksum,2 \
--place_holder __hspeed_crc_checksum,2,.hspeed_crc_checksum,2 \
--place_holder __dflash_crc_checksum,2,.dflash_crc_checksum,2 \
--log libraries,initialization,modules,redirects,sections,unused_fragments,call_graph \
--log_file ${target_name}.log \
--remarks \
--debug_lib "
        )
    else()
        set_target_properties(${LINK_TARGET_NAME} PROPERTIES
            LINK_FLAGS " --map ${LINK_FILE_BASENAME}.map \
--config ${linker_file} \
--config_def _NEAR_HEAP_SIZE=0 \
--config_def _FAR_HEAP_SIZE=0 \
--config_def _HUGE_HEAP_SIZE=0 \
--config_def _STACK_SIZE=1280 \
--config_def _NEAR_CONST_LOCATION_SIZE=0x4F00 \
--config_def _NEAR_CONST_LOCATION_START=0x5000 \
--define_symbol _NEAR_CONST_LOCATION=0 \
--entry __iar_program_start \
--text_out locale \
--no_free_heap \
--log libraries,initialization,modules,redirects,sections,unused_fragments,call_graph \
--log_file ${LINK_FILE_BASENAME}.log \
--remarks \
--debug_lib "
        )
    endif()
endfunction(compiler_iar_set_target_properties)
