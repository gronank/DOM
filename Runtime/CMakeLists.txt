project(Runtime)
include(GenerateExportHeader)
# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Runtime SHARED runtime.cpp)
set_target_properties(Runtime PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/../bin")
target_link_libraries(Runtime $<COMPILE_ONLY:DOM>)
generate_export_header(Runtime)
target_sources(Runtime PUBLIC
	FILE_SET HEADERS
	TYPE HEADERS
	FILES
		runtime.h
)
