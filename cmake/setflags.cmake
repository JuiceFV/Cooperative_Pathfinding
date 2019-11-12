# set flags

message(STATUS "Set flags for the compiler")

if(CMAKE_CXX_COMPILER_ID MATCHES MINGW)
	message(STATUS "Seting flags for the MINGW Compiler")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -std=c++2a")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -O3")
	set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	message(STATUS "Flags for the MINGW Compiler was set")
elseif(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
	message(STATUS "Seting flags for the MSVC Compiler")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /we /O2")
	set(CMAKE_CXX_FLAGS_DEBUG   "/Od /Zi")
	set(CMAKE_CXX_FLAGS_RELEASE "/favor")
	message(STATUS "Flags for the MSVC Compiler was set")
endif()