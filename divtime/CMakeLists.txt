add_executable(divtime
        divtime.c
	divfunc.c
	divfunc.h
)

target_link_libraries(divtime pico_stdlib)
pico_add_extra_outputs(divtime)
