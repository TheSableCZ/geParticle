cmake_minimum_required (VERSION 3.11.1)

set(PROJECT_NAME "GPU_nbody_sim")
set(APP_NAME "GPU_nbody_sim")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(Qt5 COMPONENTS REQUIRED Gui)
find_package(GPUEngine COMPONENTS geGL geUtil)

set(DEFAULT_RESOERCES_PATH "${CMAKE_CURRENT_LIST_DIR}/resources")

set(${APP_NAME}_RESOURCES "${DEFAULT_RESOERCES_PATH}" CACHE PATH "Relative or absolute path to Application resources.")

set(SHADERS
	resources/shaders/vertexShader.glsl
	resources/shaders/fragmentShader.glsl
	resources/shaders/billboardGeometryShader.glsl
	resources/shaders/affector.glsl
	resources/shaders/emitter.glsl
)

SOURCE_GROUP(shaders FILES ${SHADERS})

set(SOURCES
	main.cpp
	OpenGLWindow.cpp
	OpenGLWindow.h
	particles/SimpleParticleRenderer.h
	particles/SimpleParticleRenderer.cpp
	particles/PSManager.h
	particles/PSManager.cpp
	utils/Camera.h
	utils/Camera.cpp
	particles/BallGPUEmitter.h
	particles/BallGPUEmitter.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${SHADERS})
target_link_libraries(${PROJECT_NAME} Qt5::Gui geGL geUtil glm geParticle)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
#target_compile_definitions(${PROJECT_NAME} PUBLIC 
#	"VERTEX_SHADER=\"${GPU_vertexShader}\"" "FRAGMENT_SHADER=\"${GPU_fragmentShader}\"" "GEOMETRY_SHADER=\"${GPU_geometryShader}\"" "AFFECTOR_SHADER=\"${GPU_affector}\""
#)
set_target_properties(${APP_NAME} PROPERTIES COMPILE_DEFINITIONS "APP_RESOURCES=\"${${APP_NAME}_RESOURCES}\"")

# setting up the MSVC helper var
get_target_property(Qt5dllPath Qt5::Gui IMPORTED_LOCATION_RELEASE)
get_filename_component(Qt5dllDir ${Qt5dllPath} DIRECTORY)

get_target_property(GPUEdllPath geGL IMPORTED_LOCATION_RELEASE)
get_filename_component(GPUEdllDir ${GPUEdllPath} DIRECTORY)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_GLOBAL_DllPath "${GPUEdllDir};${Qt5dllDir}")
