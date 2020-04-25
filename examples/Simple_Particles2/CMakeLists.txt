cmake_minimum_required (VERSION 3.11.1)

set(PROJECT_NAME "Simple_Particles2")
set(APP_NAME "Simple_Particles2")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(Qt5 COMPONENTS REQUIRED Gui)
find_package(GPUEngine COMPONENTS geGL geUtil)

find_file(simple2_vertexShader billboardVertexShader.glsl
	HINTS ${CMAKE_CURRENT_LIST_DIR}
)

find_file(simple2_fragmentShader fragmentShader.glsl
	HINTS ${CMAKE_CURRENT_LIST_DIR}
)

set(SOURCES
	main.cpp
	OpenGLWindow.cpp
	OpenGLWindow.h
	SimpleParticleRenderer.h
	SimpleParticleRenderer.cpp
	PSManager.h
	PSManager.cpp
	BillboardVertices.h
	Camera.h
	Camera.cpp
	CustomTypes.h
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} Qt5::Gui geGL geUtil glm geParticle)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions(${PROJECT_NAME} PUBLIC "VERTEX_SHADER=\"${simple2_vertexShader}\"" "FRAGMENT_SHADER=\"${simple2_fragmentShader}\"")

# setting up the MSVC helper var
get_target_property(Qt5dllPath Qt5::Gui IMPORTED_LOCATION_RELEASE)
get_filename_component(Qt5dllDir ${Qt5dllPath} DIRECTORY)

get_target_property(GPUEdllPath geGL IMPORTED_LOCATION_RELEASE)
get_filename_component(GPUEdllDir ${GPUEdllPath} DIRECTORY)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_GLOBAL_DllPath "${GPUEdllDir};${Qt5dllDir}")
