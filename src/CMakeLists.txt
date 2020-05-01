
add_library(spify STATIC param.cpp scalar_param.cpp vector_param.cpp map_param.cpp parser.cpp)
target_link_libraries(spify yaml-cpp)

set_target_properties(spify PROPERTIES PUBLIC_HEADER "map_param.h;param.h;parser.h;scalar_param.h;spify.h;vector_param.h;")
install(TARGETS spify yaml-cpp)

