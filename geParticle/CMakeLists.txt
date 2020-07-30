# @file CMakeLists.txt
# @brief CMake list for geParticle project. Core of particle module.
# @author Jan Sobol xsobol04

cmake_minimum_required (VERSION 3.11.1)

project(geParticle VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

set(LIB_NAME_UPPER)
string(TOUPPER ${PROJECT_NAME} LIB_NAME_UPPER)

include(GenerateExportHeader)
include(GNUInstallDirs)

set(HEADER_PATH ${CMAKE_CURRENT_LIST_DIR}/src/${PROJECT_NAME})
set(SRC_PATH ${CMAKE_CURRENT_LIST_DIR}/src/${PROJECT_NAME})

set(PARTICLES_INCLUDES
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/Export.h

	${HEADER_PATH}/ArrayOfStructsContainer.h
	${HEADER_PATH}/ComponentPool.h
	${HEADER_PATH}/ComponentSystemContainer.h
	${HEADER_PATH}/Counter.h
	${HEADER_PATH}/Particle.h
	${HEADER_PATH}/ParticleAffector.h
	${HEADER_PATH}/ParticleContainer.h
	${HEADER_PATH}/ParticleContainerIterator.h
	${HEADER_PATH}/ParticleContainerType.h
	${HEADER_PATH}/ParticleEmitter.h
	${HEADER_PATH}/ParticleEmitterBase.h
	${HEADER_PATH}/ParticleInitiator.h
	${HEADER_PATH}/ParticleRenderer.h
	${HEADER_PATH}/ParticleSystem.h
	${HEADER_PATH}/ParticleSystemManager.h
)

set(PARTICLES_SOURCES
	${SRC_PATH}/ComponentSystemContainer.cpp
	${SRC_PATH}/ParticleContainerIterator.cpp
	${SRC_PATH}/ParticleEmitterBase.cpp
	${SRC_PATH}/ParticleSystem.cpp
	${SRC_PATH}/ParticleSystemManager.cpp
)

################################################
# PACKAGES

find_package(glm REQUIRED)
find_package(GPUEngine COMPONENTS geCore REQUIRED)

#########################################
# Target

ADD_LIBRARY(${PROJECT_NAME}
	${PARTICLES_INCLUDES}
	${PARTICLES_SOURCES}
)

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

GENERATE_EXPORT_HEADER(${PROJECT_NAME}
   EXPORT_FILE_NAME "${PROJECT_NAME}/Export.h"
)

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

install(FILES
  ${PARTICLES_INCLUDES}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
  )
  
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

install(EXPORT ${PROJECT_NAME}Targets
   FILE ${PROJECT_NAME}Config.cmake
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)