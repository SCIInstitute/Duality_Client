include(GTest)

include_directories(${CMAKE_CURRENT_LIST_DIR})

file(GLOB TEST_HEADER
	${CMAKE_CURRENT_LIST_DIR}/*.h
	${CMAKE_CURRENT_LIST_DIR}/Mocks/*.h
	${CMAKE_CURRENT_LIST_DIR}/Scene/*.h)
file(GLOB TEST_SOURCE
	${CMAKE_CURRENT_LIST_DIR}/*.cpp
	${CMAKE_CURRENT_LIST_DIR}/Scene/*.cpp)

add_executable(tests ${TEST_HEADER} ${TEST_SOURCE})

target_link_libraries(tests gmock)
target_link_libraries(tests IV3Dm2-lib)