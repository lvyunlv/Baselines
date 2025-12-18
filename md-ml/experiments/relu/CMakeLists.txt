add_executable(relu_party_0 relu_party_0.cpp relu_config.h)
add_executable(relu_party_1 relu_party_1.cpp relu_config.h)

target_link_libraries(relu_party_0 ${ONLINE_LIB})
target_link_libraries(relu_party_1 ${ONLINE_LIB})
