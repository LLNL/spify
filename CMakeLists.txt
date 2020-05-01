cmake_minimum_required(VERSION 3.10)

project(spipfy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG        yaml-cpp-0.6.3
)
FetchContent_MakeAvailable(yaml-cpp)

add_custom_command(
    COMMAND ${PYTHON_EXECUTABLE} generate${IFPName}.py
    DEPENDS generate${IFPName}.py
    OUTPUT ${IFPName}.cpp ${IFPName}.h ${IFPName}Master.yml
    COMMENT "Generating code for ${IFPName}."
)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(example)

