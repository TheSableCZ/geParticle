cmake_minimum_required (VERSION 3.11.1)

project(geParticle)

set(LIB_NAME_UPPER)
string(TOUPPER ${PROJECT_NAME} LIB_NAME_UPPER)

include(GenerateExportHeader)

set(HEADER_PATH ${CMAKE_CURRENT_LIST_DIR}/src/${PROJECT_NAME})
set(SRC_PATH ${CMAKE_CURRENT_LIST_DIR}/src/${PROJECT_NAME})

set(PARTICLES_INCLUDES
	${HEADER_PATH}/AoSParticleFactory.h
	${HEADER_PATH}/BasicParticleEmitter.h
	${HEADER_PATH}/ComponentSystemContainer.h
	${HEADER_PATH}/GPUParticleAffector.h
	${HEADER_PATH}/GPUParticleContainer.h
	${HEADER_PATH}/Particle.h
	${HEADER_PATH}/ParticleAffector.h
	${HEADER_PATH}/ParticleContainer.h
	${HEADER_PATH}/ParticleEmitter.h
	${HEADER_PATH}/ParticleFactory.h
	${HEADER_PATH}/ParticleIterator.h
	${HEADER_PATH}/ParticleRenderer.h
	${HEADER_PATH}/ParticleSystem.h
	${HEADER_PATH}/PointEmitter.h
	${HEADER_PATH}/SimpleArrayOfStructsContainer.h
)

set(PARTICLES_SOURCES
	${SRC_PATH}/BasicParticleEmitter.cpp
	${SRC_PATH}/ParticleSystem.cpp
	${SRC_PATH}/SimpleArrayOfStructsContainer.cpp
)

################################################
# PACKAGES

find_package(glm REQUIRED)
find_package(GPUEngine COMPONENTS geCore)

#########################################
# Target

ADD_LIBRARY(${PROJECT_NAME}
	${PARTICLES_INCLUDES}
	${PARTICLES_SOURCES}
)

#GENERATE_EXPORT_HEADER(${PROJECT_NAME}
#   EXPORT_FILE_NAME "${PROJECT_NAME}/Export.h"
#)

# since we need to specify the ste library as the interface dependency for the export scripts to propagate it but we need cmake to know that this will be a target and not ste.lib since we need it later in the examples in this build tree
#add_library(ste INTERFACE IMPORTED)

target_link_libraries(${PROJECT_NAME}
   PUBLIC geCore glm
   #INTERFACE ste
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/${PROJECT_NAME}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
