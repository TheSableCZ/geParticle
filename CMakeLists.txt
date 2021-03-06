﻿cmake_minimum_required (VERSION 3.11.1)

PROJECT(geParticle
   VERSION 0.1
   DESCRIPTION "Particle modul for GPUEngine"
   LANGUAGES CXX
)

# Dynamic vs Static Linkage
option(BUILD_SHARED_LIBS "Build GPUEngine as libs for dynamic linkage. Use OFF for static libs." ON)

SET(GEPARTICLE_BUILD_EXAMPLES ON CACHE BOOL "Build examples (examples directory).")
SET(GEPARTICLE_BUILD_GL ON CACHE BOOL "Build geParticleGL.")
SET(GEPARTICLE_BUILD_STD ON CACHE BOOL "Build geParticleStd.")

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_STANDARD 99)

SET(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib)
SET(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin)

##################################################
#Set the output to one set of directories lib/ bin/
##################################################
FOREACH(CONF ${CMAKE_CONFIGURATION_TYPES})        # For each configuration (Debug, Release, MinSizeRel... and/or anything the user chooses)
   STRING(TOUPPER "${CONF}" CONF)                # Go uppercase (DEBUG, RELEASE...)
   SET("CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONF}" "${OUTPUT_LIBDIR}/${CONF}")
   SET("CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONF}" "${OUTPUT_BINDIR}/${CONF}")
   IF(WIN32)
       SET("CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONF}" "${OUTPUT_LIBDIR}/${CONF}")
   ELSE()
       SET("CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONF}" "${OUTPUT_LIBDIR}/${CONF}")
   ENDIF()
ENDFOREACH()

add_subdirectory ("geParticle")

if(GEPARTICLE_BUILD_GL)
  add_subdirectory ("geParticleGL")
endif()

if(GEPARTICLE_BUILD_STD)
  add_subdirectory ("geParticleStd")
endif()

if(GEPARTICLE_BUILD_EXAMPLES)
  add_subdirectory ("examples")
endif()
