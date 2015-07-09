cmake_minimum_required(VERSION 3.1)

# Useful commands 
set(INVOKE_CMAKE_SCRIPT ${CMAKE_COMMAND} -DLOCAL_SOURCE_DIR="${CMAKE_SOURCE_DIR}" -P) 
set(INVOKE_CMAKE_SCRIPT_HARDCODED ${CMAKE_COMMAND} -DLOCAL_SOURCE_DIR="${CMAKE_SOURCE_DIR}" -DVERSION_HARDCODED=TRUE -P) 
set(COPY_DIFF ${CMAKE_COMMAND} -E copy_if_different) 
set(COPY_DIR ${CMAKE_COMMAND} -E copy_directory) 
set(REMOVE_DIR ${CMAKE_COMMAND} -E remove_directory)
set(REMOVE ${CMAKE_COMMAND} -E remove -f)
set(ENV_CMD ${CMAKE_COMMAND} -E env ${EXTRA_ARG})

#### Git Info ####
add_subdirectory(${PROJECT_SOURCE_DIR}/template ${PROJECT_BINARY_DIR}/src_template)
if(VERSION_HARDCODED)
  
  add_custom_target(version ${INVOKE_CMAKE_SCRIPT_HARDCODED} ${PROJECT_SOURCE_DIR}/template/CMakeLists.txt 
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )

else()
  # clean & regenerate git commit hash
  add_custom_target(clean_version ${REMOVE} ${PROJECT_SOURCE_DIR}/src/version.h
   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
  add_custom_target(version ${INVOKE_CMAKE_SCRIPT} ${PROJECT_SOURCE_DIR}/template/CMakeLists.txt
    DEPENDS clean_version 
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
endif(VERSION_HARDCODED)

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
  add_executable(${CMAKE_PROJECT_NAME} main.cpp version.h)
  #target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES})
  add_dependencies(${CMAKE_PROJECT_NAME} version)

  # make install
  install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${PROJECT_SOURCE_DIR}/bin)

 else()  
  message(STATUS "Boost is needed to build executables")
 endif(Boost_FOUND)

