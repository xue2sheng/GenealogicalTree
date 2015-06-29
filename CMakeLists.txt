cmake_minimum_required(VERSION 3.1)

############# PROJECT ####################

if(APPLE)
  ### Use OSX Homebrew gcc & boost
  message(STATUS "CMAKE_CXX_COMPILER should be \"/usr/local/Cellar/gcc/5.1.0/bin/g++-5\" or similar")
  set(EXTRA_ARG "PATH=\"$ENV{PATH}:/usr/texbin\"") # pdflatex
 
elseif(UNIX)
  ### Risky bet because it could be Solaris
  message(STATUS "Taking from granted this is a Linux box with evince installed")
  
elseif(WIN32)
  ### Supposed BOOST_ROOT, BOOST_INCLUDE & BOOST_LIB defined, i.e. C:\MinGW, C:\MinGW\include && C:\MinGW\lib 
  message(STATUS "CMAKE_CXX_COMPILER should be \"C:/MinGW/bin/g++.exe\" or similar")

  ### Windows default make not good enough
  if(NOT ${CMAKE_GENERATOR} MATCHES "MSYS Makefiles") # another similar option "MinGW Makefiles"
    message(WARNING "Expected MinGW generator: cmake -G \"MSYS Makefiles\" ..")
  endif()

else()
  ### Just in case
  message(FATAL_ERROR "O.S. ${CMAKE_SYSTEM} NOT SUPPORTED")

endif(APPLE)

#### C++ Project Name ###
project(GenealogicalTree CXX)

### Modern C++, GNU compiler ###
message(STATUS "CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4.9.2 (some C++14 features)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9.2)
        message(FATAL_ERROR "GCC version must be at least 4.9.2!")
    endif()
else()
    message(FATAL_ERROR "You are using an unsupported compiler! Compilation has only been tested with g++.")
endif()

# Useful commands 
set(INVOKE_CMAKE_SCRIPT ${CMAKE_COMMAND} -P) 
set(COPY_DIFF ${CMAKE_COMMAND} -E copy_if_different) 
set(REMOVE_DIR ${CMAKE_COMMAND} -E remove_directory)
set(REMOVE ${CMAKE_COMMAND} -E remove -f)
set(ENV_CMD ${CMAKE_COMMAND} -E env ${EXTRA_ARG})

#### Git Info ####
add_subdirectory(${PROJECT_SOURCE_DIR}/template)
# clean & regenerate git commit hash
add_custom_target(clean_git_commit_hash ${REMOVE} ${PROJECT_SOURCE_DIR}/Doxyfile
  COMMAND ${REMOVE} ${PROJECT_SOURCE_DIR}/src/version.h
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/template
)
add_custom_target(git_commit_hash ${INVOKE_CMAKE_SCRIPT} ${PROJECT_SOURCE_DIR}/template/CMakeLists.txt
  DEPENDS clean_git_commit_hash 
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/template
)

############# DOCUMENTATION ##############

find_package(Doxygen)
if(DOXYGEN_FOUND)

   # TARGET
   set(PDF_FILE ${PROJECT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.pdf)

   if(APPLE)

     ### PlantUML
     if($ENV{PLANTUMLJAR})
       set(PLANTUML java -jar $ENV{PLANTUMLJAR})
     else()
       set(PLANTUML java -jar /opt/plantuml/plantuml.jar)
     endif()

     ### PDF Reader 
     if($ENV{PDF_READER})
       set(PDF_READER $ENV{PDF_READER})
     else()
       set(PDF_READER "Preview")
     endif()
    
     ### Utility to show pdfs
     set(UTILITY  "$ENV{HOME}/show") 
     file(WRITE ${UTILITY} "#!/bin/bash\n")
     file(APPEND ${UTILITY} "make show -f ${PROJECT_BINARY_DIR}/Makefile -C ${PROJECT_BINARY_DIR} > /dev/null 2>&1\n")
     message(STATUS "Created ~/show utility")
     
   elseif(UNIX)

     ### PlantUML
     if($ENV{PLANTUMLJAR})
       set(PLANTUML java -jar $ENV{PLANTUMLJAR})
     else()
       set(PLANTUML java -jar /opt/plantuml/plantuml.jar)
     endif()

     ### PDF Reader 
     if($ENV{PDF_READER})
       set(PDF_READER $ENV{PDF_READER})
     else()
       set(PDF_READER "evince")
     endif()
    
     ### Utility to show pdfs
     set(UTILITY  "$ENV{HOME}/show") 
     file(WRITE ${UTILITY} "#!/bin/bash\n")
     file(APPEND ${UTILITY} "make show -f ${PROJECT_BINARY_DIR}/Makefile -C ${PROJECT_BINARY_DIR} > /dev/null 2>&1\n")
     message(STATUS "Created ~/show utility")

   elseif(WIN32)

     ### PlantUML
     if($ENV{PLANTUMLJAR})
      set(PLANTUML java -jar $ENV{PLANTUMLJAR})
     else()
      set(PLANTUML java -jar C:/strawberry/perl/site/bin/plantuml.jar)
     endif()

     ### PDF Reader 
     if($ENV{PDF_READER})
       set(PDF_READER $ENV{PDF_READER})
     else()
       set(PDF_READER "C:\\Program Files (x86)\\Adobe\\Reader 11.0\\Reader\\AcroRd32.exe")
     endif()
    
     ### Utility to show pdfs
     set(UTILITY  "$ENV{USERPROFILE}/show.bat") 
     file(WRITE ${UTILITY} "@echo off\n")
     file(APPEND ${UTILITY} "echo Generating documentation ...\n")
     file(APPEND ${UTILITY} "make doc -f ${PROJECT_BINARY_DIR}/Makefile -C ${PROJECT_BINARY_DIR} > NUL 2>&1\n")
     file(APPEND ${UTILITY} "echo Documentation generated\n")
     file(APPEND ${UTILITY} "start /I \"${PDF_READER}\" \"${PDF_FILE}\"\n")
     message(STATUS "Created ~/show utility")

 endif()

 # make doc
 add_custom_target( doc ${PLANTUML} README.md src/ test/ template/
   COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/Doxyfile
   COMMAND ${ENV_CMD} ${CMAKE_BUILD_TOOL} -f ${PROJECT_SOURCE_DIR}/latex/Makefile -C ${PROJECT_SOURCE_DIR}/latex
   COMMAND ${COPY_DIFF} ${PROJECT_SOURCE_DIR}/latex/refman.pdf ${PDF_FILE}
   COMMAND ${REMOVE_DIR} ${PROJECT_SOURCE_DIR}/latex
   DEPENDS git_commit_hash 
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
 )
 
 # make show
 if(APPLE)
   add_custom_target( show open -a ${PDF_READER} ${PDF_FILE} DEPENDS doc )
 elseif(UNIX)
   add_custom_target( show ${PDF_READER} ${PDF_FILE} DEPENDS doc )
 elseif(WIN32)
   message(STATUS "Not fully support for \"make show\" on Windows") # due to error messages after closing that PDF 
   add_custom_target( show echo "run ~/show (Powershell) or %USERPROFILE%/show (MSDOS)" DEPENDS doc )
 endif()
	
endif(DOXYGEN_FOUND)

########## BINARIES ###################

# Boost required for 'make' and 'make install'
find_package(Boost)
if(Boost_FOUND)

  # make
  add_definitions(-std=c++14)
  include_directories(${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src)
  add_executable(${CMAKE_PROJECT_NAME} src/main.cpp src/version.h)
  add_dependencies(${CMAKE_PROJECT_NAME} git_commit_hash)
  #target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES})

  # make install
  install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${PROJECT_SOURCE_DIR}/bin)

else()  
  message(STATUS "Boost is needed to build executables")

endif(Boost_FOUND)

