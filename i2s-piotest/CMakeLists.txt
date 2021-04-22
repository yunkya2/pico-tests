add_executable(i2s-piotest)

pico_generate_pio_header(i2s-piotest ${CMAKE_CURRENT_LIST_DIR}/i2s.pio)

target_sources(i2s-piotest PRIVATE i2s-piotest.c)

target_link_libraries(i2s-piotest PRIVATE pico_stdlib hardware_pio)
pico_add_extra_outputs(i2s-piotest)
