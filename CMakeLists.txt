cmake_minimum_required (VERSION 3.11.1)

PROJECT(geParticle
   VERSION 0.1
   DESCRIPTION "Particle modul for GPUEngine"
   LANGUAGES CXX
)

SET(GEPARTICLE_BUILD_EXAMPLES ON CACHE BOOL "Build examples (examples directory).")

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_STANDARD 99)


add_subdirectory ("geParticle")

# TODO: podmínit
add_subdirectory ("geParticleGL")
add_subdirectory ("geParticleStd")

if(GEPARTICLE_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
