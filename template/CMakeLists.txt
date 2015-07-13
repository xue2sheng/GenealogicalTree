cmake_minimum_required(VERSION 3.1)

# Useful commands 
set(INVOKE_CMAKE_SCRIPT ${CMAKE_COMMAND} -P) 
set(COPY_DIFF ${CMAKE_COMMAND} -E copy_if_different) 
set(COPY_DIR ${CMAKE_COMMAND} -E copy_directory) 
set(REMOVE_DIR ${CMAKE_COMMAND} -E remove_directory)
set(REMOVE ${CMAKE_COMMAND} -E remove -f)
set(ENV_CMD ${CMAKE_COMMAND} -E env ${EXTRA_ARG})

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
 # It's expected a LOCAL_SOURCE_DIR=<valid path> commandline parameter
 set(TEMPLATE_DIR "${LOCAL_SOURCE_DIR}/template")
 set(SRC_DIR "${LOCAL_SOURCE_DIR}/src")
else()
 set(TEMPLATE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
 set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
endif()

## Working directories ##
set(GIT_DIR "${CMAKE_SOURCE_DIR}")
set(DOXYFILE_IN "${TEMPLATE_DIR}/Doxyfile.in")
set(DOXYFILE_OUT "${CMAKE_BINARY_DIR}/Doxyfile")
set(HEADER_IN "${TEMPLATE_DIR}/header.tex.in")
set(CMAKE_HEADER_OUT "${CMAKE_BINARY_DIR}/header.tex")

if(VERSION_HARDCODED)
   set(VERSION_IN "${TEMPLATE_DIR}/version_hardcoded.h.in")
else()
   set(VERSION_IN "${TEMPLATE_DIR}/version.h.in")
endif()
set(VERSION_OUT "${SRC_DIR}/version.h") 

## System Info & escape string 
if(WIN32)
 set(GIT_SYSTEM "$ENV{USERNAME}@$ENV{COMPUTERNAME}")
 #set(ESC "\\\\\\")
else()
 site_name(CMAKE_HOSTNAME)
 set(GIT_SYSTEM "$ENV{USER}@${CMAKE_HOSTNAME}")
 set(ESC "\\")
endif(WIN32)

if(VERSION_HARDCODED)

  ### FAKE info ###
  set(CMAKE_VERSION_FIRST "0")
  set(CMAKE_VERSION_MIDDLE "0")
  set(CMAKE_VERSION_LAST "0")
  set(CMAKE_GIT_DETAILS "0123456 (HEAD, origin/develop, origin/HEAD, develop) FAKE: hardcoded values without using GIT")
  set(CMAKE_GIT_COMMIT_HASH "0123456")

  message(STATUS "Using harcoded values, i.e. 0123456, at generated version.h")
else()

 ### Human Info ###
 set(CMAKE_VERSION_FIRST "0")
 set(CMAKE_VERSION_MIDDLE "0")
 set(CMAKE_VERSION_LAST "2")

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

 message(STATUS "Using GIT values at generated version.h")
endif(VERSION_HARDCODED)

# Extra variables to be used in those templates

set(DOXYFILE_PROJECT_NUMBER "${CMAKE_VERSION_FIRST}.${CMAKE_VERSION_MIDDLE}.${CMAKE_VERSION_LAST}")
set(DOXYFILE_PROJECT_NAME "Genealogical Tree")
set(DOXYFILE_PROJECT_BRIEF "Find all the descendant on any level of ancestry")

# Folders to process
set(DOXYFILE_INPUT00 "${LOCAL_SOURCE_DIR}")
set(DOXYFILE_INPUT01 "${LOCAL_SOURCE_DIR}/doc")
set(DOXYFILE_INPUT02 "${LOCAL_SOURCE_DIR}/image")
set(DOXYFILE_INPUT03 "${LOCAL_SOURCE_DIR}/template")
set(DOXYFILE_INPUT04 "${LOCAL_SOURCE_DIR}/test")
set(DOXYFILE_INPUT05 "${LOCAL_SOURCE_DIR}/optimize")
set(DOXYFILE_INPUT06 "${LOCAL_SOURCE_DIR}/mock")
set(DOXYFILE_INPUT07 "${LOCAL_SOURCE_DIR}/src")
#set(DOXYFILE_INPUT08 "${LOCAL_SOURCE_DIR}")
#set(DOXYFILE_INPUT09 "${LOCAL_SOURCE_DIR}")
#set(DOXYFILE_INPUT10 "${LOCAL_SOURCE_DIR}")
#set(DOXYFILE_INPUT11 "${LOCAL_SOURCE_DIR}")
#set(DOXYFILE_INPUT12 "${LOCAL_SOURCE_DIR}")
#set(DOXYFILE_INPUT13 "${LOCAL_SOURCE_DIR}")
#set(DOXYFILE_INPUT14 "${LOCAL_SOURCE_DIR}")
#set(DOXYFILE_INPUT15 "${LOCAL_SOURCE_DIR}")
#set(DOXYFILE_INPUT16 "${LOCAL_SOURCE_DIR}")
#set(DOXYFILE_INPUT17 "${LOCAL_SOURCE_DIR}")
#set(DOXYFILE_INPUT18 "${LOCAL_SOURCE_DIR}")
#set(DOXYFILE_INPUT19 "${LOCAL_SOURCE_DIR}")

#### Transform README.md syntax into Doxygen latex 
# Choose as index page the markdown file which got as title the project name
set(INDEX_FILTER  "-e \\\"s/^# \\(${DOXYFILE_PROJECT_NAME}\\)\\(.*\\)$/# \\1 {#index}/g\\\"")
# Transform markdown image links into latex & html ones
set(IMAGE_FILTER  "-e \\\"s/^\\!\\[\\(.*\\)\\](.*\\/\\(.*\\.png\\))$/\\\\\\\\image latex \\2 \\1 \\\\\\\\image html \\2/g\\\"")
# Add second markdown levels <h2> as latex section for table of contents
set(SECTION_FILTER "-e \\\"s/^## \\([a-zA-Z0-1]\\)\\(.*\\)$/\\\\\\\\section section\\1 \\1\\2/g\\\"")
# Add third markdown levels <h3> as latex subsection for table of contents
set(SUBSECTION_FILTER "-e \\\"s/^### \\([a-zA-Z0-1]\\)\\(.*\\)$/\\\\\\\\subsection subsection\\1 \\1\\2/g\\\"") 
# all together
set(DOXYFILE_INPUT_FILTER  "sed ${INDEX_FILTER} ${IMAGE_FILTER} ${SECTION_FILTER} ${SUBSECTION_FILTER}") 

#message(FATAL_ERROR "DOXYFILE_INPUT_FILTER=${DOXYFILE_INPUT_FILTER}")

## WARNING: LaTeX special characteres, as 'underscore', must be avoided
string(REGEX REPLACE "_" "-" CMAKE_GIT_DETAILS ${CMAKE_GIT_DETAILS})
string(REGEX REPLACE "&" " and " CMAKE_GIT_DETAILS ${CMAKE_GIT_DETAILS})
string(REGEX REPLACE "_" "-" GIT_SYSTEM ${GIT_SYSTEM})
string(REGEX REPLACE "&" " and " GIT_SYSTEM ${GIT_SYSTEM})

configure_file(${VERSION_IN} ${VERSION_OUT})
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT})
configure_file(${HEADER_IN} ${CMAKE_HEADER_OUT})

message(STATUS "GIT_DETAILS=\"${CMAKE_GIT_DETAILS}\"")
message(STATUS "GIT_SYSTEM=\"${GIT_SYSTEM}\"")

