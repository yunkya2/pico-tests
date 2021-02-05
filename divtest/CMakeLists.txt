add_executable(divtest
        main.c
)

target_link_libraries(divtest pico_stdlib)
pico_add_extra_outputs(divtest)
