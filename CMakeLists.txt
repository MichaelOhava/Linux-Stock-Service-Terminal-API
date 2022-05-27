project(Stocks_API)

link_directories($(CMAKE_BINARY_DIR)/res)

add_subdirectory(src)

add_library(
	Help-Functions SHARED
	src/HelpFunctions.h
	src/HelpFunctions.c
	)
add_library(
	Operations SHARED
	src/Operations.h
	src/Operations.c
	)
	
add_executable(run_stocks_api src/Main.c)

target_link_libraries(run_stocks_api PRIVATE Operations Help-Functions json-c zip)
