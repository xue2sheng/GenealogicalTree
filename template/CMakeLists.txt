cmake_minimum_required(VERSION 3.1)

# Useful commands 
set(INVOKE_CMAKE_SCRIPT ${CMAKE_COMMAND} -P) 
set(COPY_DIFF ${CMAKE_COMMAND} -E copy_if_different) 
set(REMOVE_DIR ${CMAKE_COMMAND} -E remove_directory)
set(REMOVE ${CMAKE_COMMAND} -E remove -f)
set(ENV_CMD ${CMAKE_COMMAND} -E env ${EXTRA_ARG})

## Working directories ##
set(GIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
set(DOXYFILE_OUT "${CMAKE_CURRENT_SOURCE_DIR}/../Doxyfile")
set(VERSION_IN "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in")
set(VERSION_OUT "${CMAKE_CURRENT_SOURCE_DIR}/../src/version.h")
set(HEADER_IN "${CMAKE_CURRENT_SOURCE_DIR}/header.tex.in")
set(HEADER_OUT "${CMAKE_CURRENT_SOURCE_DIR}/../header.tex")

## System Info
if(WIN32)
 set(GIT_SYSTEM "$ENV{USERNAME}@$ENV{COMPUTERNAME}")
else()
 set(GIT_SYSTEM "$ENV{USER}@$ENV{HOSTNAME}")
endif(WIN32)

#### Git Info ####
execute_process(
  COMMAND git log -1 --format=%h 
  WORKING_DIRECTORY ${GIT_DIR} 
  OUTPUT_VARIABLE CMAKE_GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
  COMMAND git log --oneline --decorate -1
  WORKING_DIRECTORY ${GIT_DIR} 
  OUTPUT_VARIABLE CMAKE_GIT_DETAILS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
configure_file(${VERSION_IN} ${VERSION_OUT})
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT})
configure_file(${HEADER_IN} ${HEADER_OUT})

message(STATUS "GIT_DETAILS=\"${CMAKE_GIT_DETAILS}\"")
message(STATUS "GIT_SYSTEM=\"${GIT_SYSTEM}\"")
