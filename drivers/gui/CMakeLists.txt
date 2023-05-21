add_library(drivers_gui INTERFACE)

target_sources(drivers_gui INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/Gui.cpp
        )

target_link_libraries(drivers_gui INTERFACE pico_stdlib)
