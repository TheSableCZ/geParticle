cmake_minimum_required (VERSION 3.11.1)

PROJECT(geParticle
   VERSION 0.1
   DESCRIPTION "Particle modul for GPUEngine"
   LANGUAGES CXX
)

SET(GEPARTICLE_BUILD_EXAMPLES ON CACHE BOOL "Build examples (examples directory).")
SET(GEPARTICLE_BUILD_GL ON CACHE BOOL "Build geParticleGL.")
SET(GEPARTICLE_BUILD_STD ON CACHE BOOL "Build geParticleStd.")

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_STANDARD 99)


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
