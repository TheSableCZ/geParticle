﻿# @file CMakeLists.txt
# @brief CMake list for geParticleStd project.
# @author Jan Sobol xsobol04

cmake_minimum_required (VERSION 3.11.1)

project(geParticleStd)

set(CMAKE_CXX_STANDARD 17)

set(LIB_NAME_UPPER)
string(TOUPPER ${PROJECT_NAME} LIB_NAME_UPPER)

include(GenerateExportHeader)
include(GNUInstallDirs)

set(HEADER_PATH ${CMAKE_CURRENT_LIST_DIR}/src/${PROJECT_NAME})
set(SRC_PATH ${CMAKE_CURRENT_LIST_DIR}/src/${PROJECT_NAME})

set(PARTICLES_INCLUDES
	${HEADER_PATH}/BallEmitter.h
	${HEADER_PATH}/BoxEmitter.h
	${HEADER_PATH}/ColorAffector.h
	${HEADER_PATH}/ColorInitiator.h
	${HEADER_PATH}/ConstantRateCounter.h
	${HEADER_PATH}/FloorAffector.h
	${HEADER_PATH}/GravityAffector.h
	${HEADER_PATH}/LifeTimeAffector.h
	${HEADER_PATH}/LifeTimeInitiator.h
	${HEADER_PATH}/LinearMovementAffector.h
	${HEADER_PATH}/PointEmitter.h
	${HEADER_PATH}/StandardEmitter.h
	${HEADER_PATH}/StandardParticleComponents.h
	${HEADER_PATH}/VelocityInitiator.h
)

set(PARTICLES_SOURCES
	${SRC_PATH}/BallEmitter.cpp
	${SRC_PATH}/BoxEmitter.cpp
	${SRC_PATH}/ColorAffector.cpp
	${SRC_PATH}/ColorInitiator.cpp
	${SRC_PATH}/ConstantRateCounter.cpp
	${SRC_PATH}/FloorAffector.cpp
	${SRC_PATH}/GravityAffector.cpp
	${SRC_PATH}/LifeTimeAffector.cpp
	${SRC_PATH}/LifeTimeInitiator.cpp
	${SRC_PATH}/LinearMovementAffector.cpp
	${SRC_PATH}/PointEmitter.cpp
	${SRC_PATH}/StandardEmitter.cpp
	${SRC_PATH}/VelocityInitiator.cpp
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

GENERATE_EXPORT_HEADER(${PROJECT_NAME}
   EXPORT_FILE_NAME "${PROJECT_NAME}/Export.h"
)

# since we need to specify the ste library as the interface dependency for the export scripts to propagate it but we need cmake to know that this will be a target and not ste.lib since we need it later in the examples in this build tree
#add_library(ste INTERFACE IMPORTED)

target_link_libraries(${PROJECT_NAME}
   PUBLIC geCore glm geParticle
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