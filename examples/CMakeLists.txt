set(examples_folder_name "examples")

set(examplesList
#	Simple_Particles
	Simple_Particles2
)

foreach(example IN LISTS examplesList)
   add_subdirectory(${example})
endforeach()

message(STATUS "Examples configured:")

foreach(example IN LISTS examplesList)
   if(TARGET ${example})
      message(STATUS "   ${example}")
   endif()
endforeach()
