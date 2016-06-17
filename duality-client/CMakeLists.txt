CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)
PROJECT(duality-client LANGUAGES CXX)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS on)

SET(DUALITY_HEADER_PUBLIC
	include/duality/CoordinateSystem.h
	include/duality/Error.h
	include/duality/SceneController2D.h
	include/duality/SceneController3D.h
	include/duality/SceneLoader.h
	include/duality/SceneMetadata.h
	include/duality/ScreenInfo.h
	include/duality/InputVariable.h

	include/IVDA/GLMatrix.h
	include/IVDA/StdDefines.h
	include/IVDA/Vectors.h)
	
SET(DUALITY_HEADER_PRIVATE
	src/duality/DownloadProvider.h
	src/duality/G3D.h
	src/duality/I3M.h
	src/duality/Dataset.h
	src/duality/GeometryDataset.h
	src/duality/VolumeDataset.h
	src/duality/GeometryRenderer2D.h
	src/duality/GeometryRenderer3D.h
	src/duality/VolumeRenderer3D.h
	src/duality/GeometryUtil.h
	src/duality/GeometryUtil.h
	src/duality/MVP2D.h
	src/duality/MVP3D.h
	src/duality/PythonProvider.h
	src/duality/NodeDispatcher.h
	src/duality/RenderDispatcher2D.h
	src/duality/RenderDispatcher3D.h
	src/duality/RenderParameters2D.h
	src/duality/RenderParameters3D.h
	src/duality/SceneController2DImpl.h
	src/duality/SceneController3DImpl.h
	src/duality/Scene.h
	src/duality/SceneNode.h
	src/duality/GeometryNode.h
	src/duality/VolumeNode.h
	src/duality/SceneParser.h
	src/duality/AbstractIO.h
	src/duality/BoundingBox.h
	src/duality/Communication.h
	src/duality/DataProvider.h
	src/duality/GLTexture2D.h
	src/duality/TransferFunction.h
	src/duality/View.h
	
	src/IVDA/ArcBall.h
	src/IVDA/Console.h
	src/IVDA/ConsoleOut.h
	src/IVDA/DebugOutHandler.h
	src/IVDA/GLInclude.h
	src/IVDA/iOS.h
	src/IVDA/Macros.h
	src/IVDA/MultiplexOut.h
	src/IVDA/Singleton.h
	src/IVDA/SysTools.h
	src/IVDA/Threads.h
	src/IVDA/XQueryComponents.h
	src/IVDA/AbstrDebugOut.h)
	
SET (DUALITY_SOURCE
	src/IVDA/Macros.cpp
	src/IVDA/MultiplexOut.cpp
	src/IVDA/SysTools.cpp
	src/IVDA/Threads.cpp
	src/IVDA/AbstrDebugOut.cpp
	src/IVDA/ArcBall.cpp
	src/IVDA/ConsoleOut.cpp
	src/IVDA/DebugOutHandler.cpp
	src/IVDA/GLMatrix.cpp
	src/IVDA/GLShader.cpp
	src/IVDA/GLFrameBufferObject.cpp

	src/duality/CoordinateSystem.cpp
	src/duality/GeometryRenderer2D.cpp
	src/duality/GeometryRenderer3D.cpp
	src/duality/VolumeRenderer2D.cpp
	src/duality/VolumeRenderer3D.cpp
	src/duality/Dataset.cpp
	src/duality/GeometryDataset.cpp
	src/duality/VolumeDataset.cpp
	src/duality/G3D.cpp
	src/duality/I3M.cpp
	src/duality/DownloadProvider.cpp
	src/duality/Communication.cpp
	src/duality/BoundingBox.cpp
	src/duality/AbstractIO.cpp
	src/duality/SceneController3D.cpp
	src/duality/SceneParser.cpp
	src/duality/SceneNode.cpp
	src/duality/GeometryNode.cpp
	src/duality/VolumeNode.cpp
	src/duality/SceneMetadata.cpp
	src/duality/SceneLoader.cpp
	src/duality/Scene.cpp
	src/duality/SceneController2DImpl.cpp
	src/duality/SceneController3DImpl.cpp
	src/duality/RenderDispatcher2D.cpp
	src/duality/RenderDispatcher3D.cpp
	src/duality/RenderParameters2D.cpp
	src/duality/RenderParameters3D.cpp
	src/duality/PythonProvider.cpp
	src/duality/MVP2D.cpp
	src/duality/MVP3D.cpp
	src/duality/GLTexture2D.cpp
	src/duality/TransferFunction.cpp)
	
SOURCE_GROUP("Public Headers" FILES ${DUALITY_HEADER_PUBLIC})	
SOURCE_GROUP("Private Headers" FILES ${DUALITY_HEADER_PRIVATE})

ADD_LIBRARY(duality-client ${DUALITY_HEADER_PUBLIC} ${DUALITY_HEADER_PRIVATE} ${DUALITY_SOURCE})

TARGET_INCLUDE_DIRECTORIES(
	duality-client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	duality-client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

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