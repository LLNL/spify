cmake_minimum_required(VERSION 3.12)

project(spify LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(GNUInstallDirs)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

include(FetchContent)
FetchContent_Declare(
  yaml-cpp
  URL https://github.com/jbeder/yaml-cpp/archive/0.8.0.tar.gz
)

FetchContent_GetProperties(yaml-cpp)
if(NOT yaml-cpp_POPULATED)
  FetchContent_Populate(yaml-cpp)
  #Turning off yaml-cpp tests avoids googletest
  #(Important when including spify in a cmake build that has it's own googletest)
  set(YAML_CPP_BUILD_TESTS OFF CACHE INTERNAL "")
  set(YAML_CPP_BUILD_TOOLS OFF CACHE INTERNAL "")
  add_subdirectory(
    ${yaml-cpp_SOURCE_DIR}
    ${yaml-cpp_BINARY_DIR}
    #EXCLUDE_FROM_ALL
  )
endif()

function(add_spifyIFP_target IFPName)
add_custom_command(
    COMMAND ${CMAKE_COMMAND} -E env PYTHONDONTWRITEBYTECODE=1 PYTHONPATH=${spify_SOURCE_DIR}/src ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/generate${IFPName}.py
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generate${IFPName}.py
    OUTPUT ${IFPName}.cpp ${IFPName}.h
    COMMENT "Generating code for ${IFPName}."
)

set(targets ${ARGN})
foreach(target ${targets})
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_sources(${target} PRIVATE ${IFPName}.cpp)
endforeach()
endfunction()

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(example)

