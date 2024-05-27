add_library(drivers_pwm INTERFACE)

target_sources(
        drivers_pwm INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/Pwm.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Tone.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Servo.cpp
)

target_include_directories(
        drivers_pwm INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(
        drivers_pwm INTERFACE
        pico_stdlib
        hardware_pwm
)
