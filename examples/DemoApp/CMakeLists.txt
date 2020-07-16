cmake_minimum_required (VERSION 3.11.1)

set(PROJECT_NAME "DemoApp")
set(APP_NAME "DemoApp")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(Qt5 COMPONENTS REQUIRED Gui)
find_package(GPUEngine COMPONENTS geGL geUtil)

set(DEFAULT_RESOURCES_PATH "${CMAKE_CURRENT_LIST_DIR}/resources")

set(${APP_NAME}_RESOURCES "${DEFAULT_RESOURCES_PATH}" CACHE PATH "Relative or absolute path to Application resources.")

set(SHADERS
	resources/Simple/vertexShader.glsl
	resources/Simple/fragmentShader.glsl
	resources/Simple/billboardGeometryShader.glsl
)

SOURCE_GROUP(shaders FILES ${SHADERS})

set(LIB
	lib/qtimgui/ImGuiRenderer.h
	lib/qtimgui/ImGuiRenderer.cpp
	lib/qtimgui/QtImGui.h
	lib/qtimgui/QtImGui.cpp
)
add_library(imgui
	STATIC
	lib/qtimgui/imgui/imconfig.h
	lib/qtimgui/imgui/imgui_demo.cpp
	lib/qtimgui/imgui/imgui_draw.cpp
	lib/qtimgui/imgui/imgui_internal.h
	lib/qtimgui/imgui/imgui_widgets.cpp
	lib/qtimgui/imgui/imgui.cpp
)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/qtimgui/imgui)

set(SOURCES
	main.cpp
	OpenGLWindow.cpp
	OpenGLWindow.h
	ExampleManager.h
	ExampleManager.cpp
	SimpleExample.h
	SimpleExample.cpp
	SimpleExampleRenderer.h
	SimpleExampleRenderer.cpp
	CameraSingleton.h
	CameraSingleton.cpp
#	particles/SimpleParticleRenderer.h
#	particles/SimpleParticleRenderer.cpp
#	particles/PSManager.h
#	particles/PSManager.cpp
#	utils/Camera.h
#	utils/Camera.cpp
#	particles/BallGPUEmitter.h
#	particles/BallGPUEmitter.cpp
)

add_executable(${PROJECT_NAME} ${LIB} ${SOURCES} ${SHADERS})
target_link_libraries(${PROJECT_NAME} Qt5::Gui geGL geUtil glm geParticle geParticleGL geParticleStd imgui)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
set_target_properties(${APP_NAME} PROPERTIES COMPILE_DEFINITIONS "APP_RESOURCES=\"${${APP_NAME}_RESOURCES}\"")

# setting up the MSVC helper var
get_target_property(Qt5dllPath Qt5::Gui IMPORTED_LOCATION_RELEASE)
get_filename_component(Qt5dllDir ${Qt5dllPath} DIRECTORY)

get_target_property(GPUEdllPath geGL IMPORTED_LOCATION_RELEASE)
get_filename_component(GPUEdllDir ${GPUEdllPath} DIRECTORY)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_GLOBAL_DllPath "${GPUEdllDir};${Qt5dllDir}")
