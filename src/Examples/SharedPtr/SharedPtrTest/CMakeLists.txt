make_executable()

target_include_directories(${PROJECT} PUBLIC ${Boost_INCLUDE_DIR} ${SharedPtrLib_INCLUDE_DIR})

target_link_libraries(${PROJECT} PUBLIC ${Boost_LIBRARIES} SharedPtrLib)
