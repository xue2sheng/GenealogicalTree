cmake_minimum_required(VERSION 3.1)

############# PROJECT ####################

if(ONLY_DOC)

 message(STATUS "Only Documentation project")

 if(APPLE)
  set(EXTRA_ARG "PATH=\"$ENV{PATH}:/usr/texbin\"") # pdflatex
  ### make
  if(${CMAKE_BUILD_TOOL} MATCHES ".*make$")
    message(STATUS "Using defined make")
  else()
    set(CMAKE_BUILD_TOOL "/usr/bin/make")
  endif()
  message(STATUS "CMAKE_BUILD_TOOL = ${CMAKE_BUILD_TOOL}")

 elseif(UNIX)
  ### make
  if(${CMAKE_BUILD_TOOL} MATCHES ".*make$")
    message(STATUS "Using defined make")
  else()
    set(CMAKE_BUILD_TOOL "/usr/bin/make")
  endif()
  message(STATUS "CMAKE_BUILD_TOOL = ${CMAKE_BUILD_TOOL}")
 
 elseif(WIN32)
  ### make
  if(${CMAKE_BUILD_TOOL} MATCHES ".*make.exe$")
    message(STATUS "Using defined make")
  else()
    file(TO_CMAKE_PATH "C:\\MinGW\\bin\\make.exe" CMAKE_BUILD_TOOL)
  endif()
  message(STATUS "CMAKE_BUILD_TOOL = ${CMAKE_BUILD_TOOL}")
 

  ### Windows default make not good enough
  if(NOT ${CMAKE_GENERATOR} MATCHES "MSYS Makefiles") # another similar option "MinGW Makefiles"
    message(WARNING "Expected MSYS generator: cmake -G \"MSYS Makefiles\" ..")
  endif()

 endif(APPLE)

 ### Not compiler required ###
 project(GenealogicalTree NONE)

else(ONLY_DOC)

 message(STATUS "Binary & Documentation project")

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

endif(ONLY_DOC)

# Useful commands 
set(INVOKE_CMAKE_SCRIPT ${CMAKE_COMMAND} -DLOCAL_SOURCE_DIR="${CMAKE_SOURCE_DIR}" -P) 
set(INVOKE_CMAKE_SCRIPT_HARDCODED ${CMAKE_COMMAND} -DLOCAL_SOURCE_DIR="${CMAKE_SOURCE_DIR}" -DVERSION_HARDCODED=TRUE -P) 
set(COPY_DIFF ${CMAKE_COMMAND} -E copy_if_different) 
set(REMOVE_DIR ${CMAKE_COMMAND} -E remove_directory)
set(REMOVE ${CMAKE_COMMAND} -E remove -f)
set(ENV_CMD ${CMAKE_COMMAND} -E env ${EXTRA_ARG})

#### Git Info ####
add_subdirectory(${PROJECT_SOURCE_DIR}/template)
if(VERSION_HARDCODED)
  # clean & regenerate git commit hash
  add_custom_target(clean_git_commit_hash ${REMOVE} ${PROJECT_BINARY_DIR}/Doxyfile
   COMMAND ${REMOVE} ${PROJECT_BINARY_DIR}/header.tex
   COMMAND ${REMOVE_DIR} ${PROJECT_BINARY_DIR}/latex
   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
  add_custom_target(git_commit_hash ${INVOKE_CMAKE_SCRIPT_HARDCODED} ${PROJECT_SOURCE_DIR}/template/CMakeLists.txt 
    DEPENDS clean_git_commit_hash 
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
else()
  # clean & regenerate git commit hash
  add_custom_target(clean_git_commit_hash ${REMOVE} ${PROJECT_BINARY_DIR}/Doxyfile
   COMMAND ${REMOVE} ${PROJECT_BINARY_DIR}/header.tex
   COMMAND ${REMOVE_DIR} ${PROJECT_BINARY_DIR}/latex
   COMMAND ${REMOVE} ${PROJECT_SOURCE_DIR}/src/version.h
   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
  add_custom_target(git_commit_hash ${INVOKE_CMAKE_SCRIPT} ${PROJECT_SOURCE_DIR}/template/CMakeLists.txt
    DEPENDS clean_git_commit_hash 
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
endif(VERSION_HARDCODED)

############# DOCUMENTATION ##############

find_package(Doxygen)
if(DOXYGEN_FOUND)

   # TARGET
   set(TMP_PDF_FILE ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.pdf)
   if(${DOC_PDF} MATCHES ".*pdf$")
     set(PDF_FILE "${DOC_PDF}")
   else()
     set(PDF_FILE $ENV{HOME}/${CMAKE_PROJECT_NAME}.pdf)
   endif()

   if(APPLE)

     ### PlantUML
     if($ENV{PLANTUMLJAR} MATCHES ".*jar$")
       set(PLANTUML java -jar $ENV{PLANTUMLJAR} -v -o ${CMAKE_SOURCE_DIR}/image)
     else()
       set(PLANTUML java -jar /opt/plantuml/plantuml.jar -v -o ${CMAKE_SOURCE_DIR}/image)
     endif()

     ### PDF Reader 
     if($ENV{PDF_READER} MATCHES "..*")
       set(PDF_READER "$ENV{PDF_READER}")
     else()
       set(PDF_READER "Preview")
     endif()
    
     ### Utility to show pdfs
     set(UTILITY  "${PROJECT_BINARY_DIR}/show") 
     file(WRITE ${UTILITY} "#!/bin/bash\n")
     file(APPEND ${UTILITY} "make show -f ${PROJECT_BINARY_DIR}/Makefile -C ${PROJECT_BINARY_DIR} > /dev/null 2>&1\n")
     file(COPY ${UTILITY} DESTINATION $ENV{HOME} 
          FILE_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE
                           GROUP_WRITE GROUP_READ GROUP_EXECUTE
                           WORLD_EXECUTE)
     message(STATUS "Created ~/show utility")
     
   elseif(UNIX)

     ### PlantUML
     if($ENV{PLANTUMLJAR} MATCHES ".*jar$")
       set(PLANTUML java -jar $ENV{PLANTUMLJAR} -v -o ${CMAKE_SOURCE_DIR}/image)
     else()
       set(PLANTUML java -jar /opt/plantuml/plantuml.jar -v -o ${CMAKE_SOURCE_DIR}/image)
     endif()

     ### PDF Reader 
     if($ENV{PDF_READER} MATCHES "..*")
       set(PDF_READER "$ENV{PDF_READER}")
     else()
       set(PDF_READER "evince")
     endif()
    
     ### Utility to show pdfs
     set(UTILITY  "${PROJECT_BINARY_DIR}/show") 
     file(WRITE ${UTILITY} "#!/bin/bash\n")
     file(APPEND ${UTILITY} "make show -f ${PROJECT_BINARY_DIR}/Makefile -C ${PROJECT_BINARY_DIR} > /dev/null 2>&1\n")
     file(COPY ${UTILITY} DESTINATION $ENV{HOME} 
          FILE_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE
                           GROUP_WRITE GROUP_READ GROUP_EXECUTE
                           WORLD_EXECUTE)
     message(STATUS "Created ~/show utility")

   elseif(WIN32)

     ### PlantUML
     if($ENV{PLANTUMLJAR} MATCHES ".*jar$")
      file(TO_CMAKE_PATH "$ENV{PLANTUMLJAR}" PLANTUMLJAR_CMAKE)
      set(PLANTUML java -jar ${PLANTUMLJAR_CMAKE} -v -o ${CMAKE_SOURCE_DIR}/image)
     else()
      set(PLANTUML java -jar C:/strawberry/perl/site/bin/plantuml.jar -v -o ${CMAKE_SOURCE_DIR}/image)
     endif()

     ### PDF Reader 
     if($ENV{PDF_READER} MATCHES ".*exe$")
       file(TO_CMAKE_PATH "$ENV{PDF_READER}" PDF_READER_CMAKE)
       set(PDF_READER "${PDF_READER_CMAKE}")
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

 # make image
 add_custom_target( image ${PLANTUML} README.md src/*.* optimize/*.* test/*.* template/*.* 
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
 )

 # make doc
 add_custom_target( doc ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
   COMMAND ${ENV_CMD} ${CMAKE_BUILD_TOOL} -f ${PROJECT_BINARY_DIR}/latex/Makefile -C ${PROJECT_BINARY_DIR}/latex
   COMMAND ${COPY_DIFF} ${PROJECT_BINARY_DIR}/latex/refman.pdf ${TMP_PDF_FILE}
   DEPENDS image
   DEPENDS git_commit_hash 
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
 )

 # make install_doc
 add_custom_target( install_doc ${COPY_DIFF} ${TMP_PDF_FILE} ${PDF_FILE} DEPENDS doc )

 # make show
 if(APPLE)
   add_custom_target( show open -a ${PDF_READER} ${PDF_FILE} DEPENDS install_doc )
 elseif(UNIX)
   add_custom_target( show ${PDF_READER} ${PDF_FILE} DEPENDS install_doc )
 elseif(WIN32)
   message(STATUS "Not fully support for \"make show\" on Windows") # due to error messages after closing that PDF 
   add_custom_target( show echo "run ~/show (Powershell) or %USERPROFILE%/show (MSDOS)" DEPENDS instal_doc )
 endif()
	
endif(DOXYGEN_FOUND)

if(NOT ONLY_DOC)

 ########## BINARIES ###################

 # Boost required for 'make' and 'make install'
 find_package(Boost)
 if(Boost_FOUND)

  # make
  add_definitions(-std=c++14)

  if(WIN32) # warning: 'template<class> class std::auto_ptr' is deprecated
   add_definitions(-Wno-deprecated) 
  endif()

  include_directories(${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src)
  add_executable(${CMAKE_PROJECT_NAME} src/main.cpp src/version.h)
  #target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES})
  add_dependencies(${CMAKE_PROJECT_NAME} git_commit_hash)

  # make install
  install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${PROJECT_SOURCE_DIR}/bin)

 else()  
  message(STATUS "Boost is needed to build executables")
 endif(Boost_FOUND)

endif(NOT ONLY_DOC)
