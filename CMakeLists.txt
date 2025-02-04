#Compile all
cmake_minimum_required(VERSION 3.5)

project(Dynamix_note_width_changer)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set include directories
include_directories ("${PROJECT_SOURCE_DIR}/include")

#set output directories
# message("CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) #Archives
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) #Libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) #Apps

#compile library
message("Building libraries...")
add_subdirectory("./Chartstore")

option(BUILD_CLI "Build command line utility" ON)
option(BUILD_GUI "Build graphical application" ON)

#CLI application option
if(BUILD_CLI)
#compile CLI application
message("Building command line utility...")
add_subdirectory("./Dynamix_chart_width_control")

#install CLI application
if(UNIX)
	# Installation rules for UNIX-based systems
	set(BIN_NAME "dynamix-chart-width-control")
	install(TARGETS ${BIN_NAME} 
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
	install(FILES ${PROJECT_SOURCE_DIR}/README.md DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/${BIN_NAME})
endif()

endif()

#uninstall CLI application
if(UNIX)
	#install_manifest.txt is a list of files installed
	add_custom_target(uninstall
		COMMAND "xargs" "rm" "<" "${CMAKE_BINARY_DIR}/install_manifest.txt"
	)
endif()

#GUI application option
if(BUILD_GUI)
#compile GUI program
#Build in 64-bit environments only
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	message("Building graphical application...")
	add_subdirectory("./Dynamix_chart_width_control_GUI")
else()
	message("The GUI program cannot be compiled in a 32-bit environment.")
endif()
endif()