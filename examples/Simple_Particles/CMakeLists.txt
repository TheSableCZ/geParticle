cmake_minimum_required (VERSION 3.11.1)

set(PROJECT_NAME "Simple_Particles")
set(APP_NAME "Simple_Particles")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(Qt5 COMPONENTS REQUIRED Gui)
find_package(GPUEngine COMPONENTS geGL geUtil)

find_file(vertexShader vertexShader.glsl
	HINTS ${CMAKE_CURRENT_LIST_DIR}
)

find_file(fragmentShader fragmentShader.glsl
	HINTS ${CMAKE_CURRENT_LIST_DIR}
)

set(SOURCES
	main.cpp
	OpenGLWindow.cpp
	OpenGLWindow.h
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} Qt5::Gui geGL geUtil glm geParticle)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions(${PROJECT_NAME} PUBLIC "VERTEX_SHADER=\"${vertexShader}\"" "FRAGMENT_SHADER=\"${fragmentShader}\"")

# setting up the MSVC helper var
get_target_property(Qt5dllPath Qt5::Gui IMPORTED_LOCATION_RELEASE)
get_filename_component(Qt5dllDir ${Qt5dllPath} DIRECTORY)
message("Qt Path??? ${Qt5dllDir}")

get_target_property(GPUEdllPath geGL IMPORTED_LOCATION_RELEASE)
get_filename_component(GPUEdllDir ${GPUEdllPath} DIRECTORY)
message("GPUE Path??? ${GPUEdllDir}")

set_target_properties(${PROJECT_NAME} PROPERTIES VS_GLOBAL_DllPath "${GPUEdllDir};${Qt5dllDir}")
