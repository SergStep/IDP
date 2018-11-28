make_test()

target_include_directories(${PROJECT} PUBLIC ${SharedPtrLib_INCLUDE_DIR})
target_link_libraries(${PROJECT} PUBLIC SharedPtrLib)
