project(SampleLibrary)
include(GenerateExportHeader)
# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(SampleLibrary SHARED sample_c_interface.cpp sample_dom.cpp)
generate_export_header(SampleLibrary)
set_target_properties(SampleLibrary PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/../bin")

target_compile_definitions(SampleLibrary PUBLIC SampleLibrary)
target_link_libraries(SampleLibrary $<COMPILE_ONLY:DOM>)
target_sources(SampleLibrary PRIVATE
	FILE_SET private_header
	TYPE HEADERS
	FILES
		internal.hpp
		samplelibrary_export.h
)
target_sources(SampleLibrary PUBLIC
	FILE_SET HEADERS
	TYPE HEADERS
	FILES
		sample_c_interface.h
		sample_dom.h
)

