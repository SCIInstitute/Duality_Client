CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
PROJECT(duality-client LANGUAGES CXX)

OPTION(DUALITY_BUILD_RENDERERS "Build renderers" ON)
IF (DUALITY_BUILD_RENDERERS)
	SET (RENDERER_SOURCE 
		src/duality/private/GLShader.cpp
		src/duality/private/AbstractRendererImpl.cpp
		src/duality/private/GeometryRendererImpl.cpp
		src/duality/private/GLFrameBufferObject.cpp)
ENDIF ()

ADD_LIBRARY(duality-client
	src/IVDA/Macros.cpp
	src/IVDA/MultiplexOut.cpp
	src/IVDA/SysTools.cpp
	src/IVDA/Threads.cpp
	src/IVDA/AbstrDebugOut.cpp
	src/IVDA/ArcBall.cpp
	src/IVDA/ConsoleOut.cpp
	src/IVDA/DebugOutHandler.cpp

	src/duality/GLMatrix.cpp
	src/duality/GeometryRenderer.cpp
	src/duality/GeometryDataset.cpp
	src/duality/G3D.cpp
	src/duality/DownloadProvider.cpp
	src/duality/Dataset.cpp
	src/duality/Communication.cpp
	src/duality/BoundingBoxCalculator.cpp
	src/duality/AbstractRenderer.cpp
	src/duality/AbstractIO.cpp
	src/duality/SceneParser.cpp
	src/duality/SceneNode.cpp
	src/duality/SceneMetadata.cpp
	src/duality/SceneLoader.cpp
	src/duality/Scene.cpp
	src/duality/RenderDispatcher.cpp
	src/duality/PythonProvider.cpp
	src/duality/InputVariable.cpp
	${RENDERER_SOURCE})

TARGET_INCLUDE_DIRECTORIES(
	duality-client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	duality-client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

TARGET_COMPILE_FEATURES(duality-client
    PUBLIC cxx_auto_type
	PUBLIC cxx_lambdas
	PUBLIC cxx_nullptr
	PUBLIC cxx_override
	PUBLIC cxx_range_for
	PUBLIC cxx_rvalue_references
	PUBLIC cxx_static_assert
	PUBLIC cxx_strong_enums
	PUBLIC cxx_uniform_initialization)
	
TARGET_LINK_LIBRARIES(duality-client
	PUBLIC mocca)