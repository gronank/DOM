project(DOM)
include(GenerateExportHeader)
# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(DOM SHARED dom.cpp)
set_target_properties(DOM PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/../bin")
target_link_libraries(DOM PUBLIC Runtime)
generate_export_header(DOM)
target_sources(DOM PUBLIC
	FILE_SET HEADERS
	TYPE HEADERS
	FILES
		dom.h
		dom_interface.h
		dom_export.h
)
