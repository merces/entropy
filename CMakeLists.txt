# CMakeList.txt : CMake project for entropy, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("entropy")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable (entropy "entropy.cpp"  )

if (WIN32)
	set_target_properties(entropy PROPERTIES LINK_FLAGS "/link setargv.obj")
endif()

# TODO: Add tests and install targets if needed.
