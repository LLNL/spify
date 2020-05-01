
foreach(TEST_NAME Simple00IFP Simple01IFP ComplexIFP)
add_executable(test${TEST_NAME}  test${TEST_NAME}.cpp ${TEST_NAME}.cpp)
target_link_libraries(test${TEST_NAME} spify)

add_custom_command(TARGET test${TEST_NAME}  PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${CMAKE_CURRENT_SOURCE_DIR}/test${TEST_NAME}.yml
                   $<TARGET_FILE_DIR:test${TEST_NAME}>)
endforeach(TEST_NAME) 

