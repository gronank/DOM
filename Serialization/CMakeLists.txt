project(Serialization)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Serialization SHARED serialization.cpp)
generate_export_header(Serialization)
set_target_properties(Serialization PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/../bin")

target_sources(Serialization PUBLIC
	FILE_SET HEADERS
	TYPE HEADERS
	FILES
		serialization.h
		serialization_export.h
		nlohmann/json.hpp
)

target_link_libraries(Serialization PUBLIC DOM Runtime)




