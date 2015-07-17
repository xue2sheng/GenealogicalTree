cmake_minimum_required(VERSION 3.1)

############# PROJECT ####################

message(STATUS "Only Documentation project")

if(APPLE)
  set(EXTRA_ARG "PATH=\"$ENV{PATH}:/usr/texbin\"") # pdflatex
 
elseif(UNIX)
  ### Risky bet because it could be Solaris
  message(STATUS "Taking from granted this is a Linux box with evince installed")
  
elseif(WIN32)

  ### Windows default make not good enough
  if(NOT ${CMAKE_GENERATOR} MATCHES "MSYS Makefiles") # another similar option "MinGW Makefiles"
    message(WARNING "Expected MinGW generator: cmake -G \"MSYS Makefiles\" ..")
  endif()

else()
  ### Just in case
  message(FATAL_ERROR "O.S. ${CMAKE_SYSTEM} NOT SUPPORTED")

endif(APPLE)

### Not compiler required ###
project(GenealogicalTree NONE)

############# DOCUMENTATION ##############
add_subdirectory( doc )

