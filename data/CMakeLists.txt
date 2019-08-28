cmake_minimum_required(VERSION 3.5)

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	install(FILES index.html DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_library(preload_files INTERFACE )
	set(FILE_FLAGS "-s --preload-file ${CMAKE_SOURCE_DIR}/data/sol.json@sol.json" PARENT_SCOPE)
else()
	install(FILES sol.json DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()