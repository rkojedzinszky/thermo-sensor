set(MCU "attiny85")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmcu=${MCU}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmcu=${MCU}")

include_directories("${CMAKE_CURRENT_LIST_DIR}")

add_subdirectory(interrupt)

add_library("${MCU}"
	usi.cpp
	vcc.cpp
)
target_link_libraries("${MCU}" "${MCU}_interrupt")
