include(GTest)

file(GLOB TEST_HEADER ${CMAKE_CURRENT_LIST_DIR}/*.h)
file(GLOB TEST_SOURCE ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

add_executable(tests ${TEST_HEADER} ${TEST_SOURCE})

target_link_libraries(tests gmock)
target_link_libraries(tests IV3Dm2-lib)