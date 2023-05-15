function(add_example TARGET PATH_UF2)
    add_custom_target("${TARGET}---flash-it"
            DEPENDS ${TARGET}
            DEPENDS ${PATH_UF2}
            COMMAND ${CMAKE_SOURCE_DIR}/scripts/flash.sh ${PATH_UF2}
            )
endfunction()

add_subdirectory(blink)
add_subdirectory(knob_ec11)
