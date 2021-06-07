cmake_minimum_required(VERSION 3.12)

project(spify LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(GNUInstallDirs)
find_package(Python)

include(FetchContent)
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG        yaml-cpp-0.6.3
  PATCH_COMMAND patch -p1 < "${CMAKE_CURRENT_LIST_DIR}/cmake/yaml-cpp-0.6.3.patch"
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
    COMMAND PYTHONDONTWRITEBYTECODE=1 PYTHONPATH=${spify_SOURCE_DIR}/src ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/generate${IFPName}.py
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

