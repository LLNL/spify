
add_library(spify STATIC param.cpp scalar_param.cpp vector_param.cpp map_param.cpp parser.cpp)
target_link_libraries(spify yaml-cpp)

set(public_headers "map_param.h;param.h;parser.h;scalar_param.h;spify.h;vector_param.h")
list(TRANSFORM public_headers PREPEND "${spify_SOURCE_DIR}/include/spify/")
set_target_properties(spify PROPERTIES PUBLIC_HEADER "${public_headers}")

target_include_directories(spify PUBLIC $<BUILD_INTERFACE:${spify_SOURCE_DIR}/include>
                                         $<INSTALL_INTERFACE:include>)

install(TARGETS spify
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spify
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES SpifyParserGenerator.py
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spify)

