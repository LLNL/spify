

add_executable(appExampleIFP appExampleIFP.cpp ExampleIFP.cpp)
target_link_libraries(appExampleIFP spify)

add_custom_command(TARGET appExampleIFP  POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${CMAKE_CURRENT_SOURCE_DIR}/ExampleIFPInput.yml
                   $<TARGET_FILE_DIR:appExampleIFP>)

add_spifyIFP_target(ExampleIFP appExampleIFP)
