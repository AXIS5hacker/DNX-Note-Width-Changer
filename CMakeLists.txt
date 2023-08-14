cmake_minimum_required(VERSION 3.5)

project(Dynamix_note_width_changer)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#Build in 64-bit environments only
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	message("Building GUI program...")
	add_subdirectory("./Dynamix_chart_width_control_GUI")
else()
	message("The GUI program cannot be compiled in a 32-bit environment.")
endif()