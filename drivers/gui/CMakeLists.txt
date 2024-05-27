add_library(drivers_gui INTERFACE)

target_sources(
        drivers_gui INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/Gui.cpp
)

target_include_directories(
        drivers_gui INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_include_directories(
        drivers_gui INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(
        drivers_gui INTERFACE
        pico_stdlib
        drivers_fonts
)
