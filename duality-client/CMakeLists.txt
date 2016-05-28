CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)
PROJECT(duality-client LANGUAGES CXX)

SET(DUALITY_HEADER 
	include/duality/DownloadProvider.h
	include/duality/Error.h
	include/duality/G3D.h
	include/duality/GeometryDataset.h
	include/duality/GeometryRenderer2D.h
	include/duality/GeometryRenderer3D.h
	include/duality/GeometrySilceRenderer.h
	include/duality/GeometryUtil.h
	include/duality/GLMatrix.h
	include/duality/InputVariable.h
	include/duality/MVP2D.h
	include/duality/MVP3D.h
	include/duality/PythonProvider.h
	include/duality/RenderDispatcher2D.h
	include/duality/RenderDispatcher3D.h
	include/duality/Scene.h
	include/duality/SceneLoader.h
	include/duality/SceneMetadata.h
	include/duality/SceneNode.h
	include/duality/SceneParser.h
	include/duality/ScreenInfo.h
	include/duality/AbstractIO.h
	include/duality/BoundingBox.h
	include/duality/Communication.h
	include/duality/CoordinateSystem.h
	include/duality/DataProvider.h
	include/duality/Dataset.h
	include/duality/DatasetDispatcher.h

	include/IVDA/ArcBall.h
	include/IVDA/Console.h
	include/IVDA/ConsoleOut.h
	include/IVDA/DebugOutHandler.h
	include/IVDA/GLInclude.h
	include/IVDA/iOS.h
	include/IVDA/Macros.h
	include/IVDA/MultiplexOut.h
	include/IVDA/Singleton.h
	include/IVDA/StdDefines.h
	include/IVDA/SysTools.h
	include/IVDA/Threads.h
	include/IVDA/Vectors.h
	include/IVDA/XQueryComponents.h
	include/IVDA/AbstrDebugOut.h)
	
SET (DUALITY_SOURCE
	src/IVDA/Macros.cpp
	src/IVDA/MultiplexOut.cpp
	src/IVDA/SysTools.cpp
	src/IVDA/Threads.cpp
	src/IVDA/AbstrDebugOut.cpp
	src/IVDA/ArcBall.cpp
	src/IVDA/ConsoleOut.cpp
	src/IVDA/DebugOutHandler.cpp
	src/IVDA/GLShader.cpp
	src/IVDA/GLFrameBufferObject.cpp

	src/duality/CoordinateSystem.cpp
	src/duality/GLMatrix.cpp
	src/duality/GeometryRenderer2D.cpp
	src/duality/GeometryRenderer3D.cpp
	src/duality/GeometryDataset.cpp
	src/duality/G3D.cpp
	src/duality/DownloadProvider.cpp
	src/duality/Dataset.cpp
	src/duality/Communication.cpp
	src/duality/BoundingBox.cpp
	src/duality/AbstractIO.cpp
	src/duality/SceneParser.cpp
	src/duality/SceneNode.cpp
	src/duality/SceneMetadata.cpp
	src/duality/SceneLoader.cpp
	src/duality/Scene.cpp
	src/duality/RenderDispatcher2D.cpp
	src/duality/RenderDispatcher3D.cpp
	src/duality/PythonProvider.cpp
	src/duality/InputVariable.cpp
	src/duality/MVP2D.cpp
	src/duality/MVP3D.cpp)

ADD_LIBRARY(duality-client ${DUALITY_HEADER} ${DUALITY_SOURCE})

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
	
TARGET_COMPILE_DEFINITIONS(duality-client PUBLIC _USE_MATH_DEFINES)
	
TARGET_LINK_LIBRARIES(duality-client
	PUBLIC mocca)