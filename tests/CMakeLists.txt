CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)
PROJECT(duality-test LANGUAGES CXX)

ADD_EXECUTABLE(duality-test
	duality/SceneNodeTest.cpp
	duality/SceneParserTest.cpp)

TARGET_INCLUDE_DIRECTORIES(duality-test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/mocks ${CMAKE_CURRENT_SOURCE_DIR}/../duality-client)
TARGET_LINK_LIBRARIES(duality-test PRIVATE duality-client gtest gmock gtest_main gmock_main)