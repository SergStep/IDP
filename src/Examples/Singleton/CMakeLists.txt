find_package( Threads )
make_executable()

target_include_directories(${PROJECT}
    PRIVATE ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(${PROJECT}
    PUBLIC ${CMAKE_THREAD_LIBS_INIT})

