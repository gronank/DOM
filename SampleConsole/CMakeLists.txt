project(SampleConsole)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SampleConsole console.cpp)
set_target_properties(SampleConsole PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/../bin")

target_link_libraries(SampleConsole PUBLIC DOM Serialization)
add_dependencies(SampleConsole SampleLibrary)



