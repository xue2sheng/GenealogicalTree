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
 set(INFO_YAML "${LOCAL_SOURCE_DIR}/info.yaml")
 set(GIT_DIR_CORE "${LOCAL_SOURCE_DIR}/core")
 set(GIT_DIR_BASE "${LOCAL_SOURCE_DIR}/base")
 set(GIT_DIR_UI "${LOCAL_SOURCE_DIR}/ui")
 set(GIT_DIR_PERSIST "${LOCAL_SOURCE_DIR}/persist")
else()
 set(TEMPLATE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
 set(INFO_YAML "${CMAKE_SOURCE_DIR}/info.yaml")
 set(GIT_DIR_CORE "${CMAKE_SOURCE_DIR}/core")
 set(GIT_DIR_BASE "${CMAKE_SOURCE_DIR}/base")
 set(GIT_DIR_UI "${CMAKE_SOURCE_DIR}/ui")
 set(GIT_DIR_PERSIST "${CMAKE_SOURCE_DIR}/persist")
endif()

## Working directories ##
set(GIT_DIR "${CMAKE_SOURCE_DIR}")
set(DOXYFILE_IN "${TEMPLATE_DIR}/Doxyfile.in")
set(DOXYFILE_OUT "${CMAKE_BINARY_DIR}/Doxyfile")
set(MD_FILTER_IN "${TEMPLATE_DIR}/md_filter.awk.in")
set(MD_FILTER_OUT "${CMAKE_BINARY_DIR}/md_filter.awk")
set(HEADER_IN "${TEMPLATE_DIR}/header.tex.in")
set(CMAKE_HEADER_OUT "${CMAKE_BINARY_DIR}/header.tex")

## System Info & escape string 
if(WIN32)
 set(GIT_SYSTEM "$ENV{USERNAME}@$ENV{COMPUTERNAME}")
 
 ## On Windows, limited support for Markdown due to issues with quoutes & paths with blanks for gawk

 ## So latex section/subsection related to markdown #/## (or h2/h1) levels shouldn't
 ## start with the same word

 string(ASCII 92 BS) # backslash \
 
 set(DOXYFILE_INPUT_FILTER "INPUT_FILTER = \"sed -e \\\"s/^# \\(Genealogical Tree\\)\\(.*\\)$/# \\1 {#index}/g\\\" -e \\\"s/^\\!\\[\\(.*\\)\\](.*\\/\\(.*\\.png\\))$/\\\\${BS}image latex \\2 \\1 \\\\${BS}image html \\2/g\\\" -e \\\"s/^## \\([a-zA-Z0-9]*\\)\\(.*\\)$/\\\\\\section section\\1 \\1\\2/g\\\" -e \\\"s/^### \\([a-zA-Z0-9]*\\)\\(.*\\)$/\\\\\\subsection subsection\\1 \\1\\2/g\\\"\"")


 set(DOXYFILE_FILTER_PATTERNS "FILTER_PATTERNS = ")

else()
 site_name(CMAKE_HOSTNAME)
 set(GIT_SYSTEM "$ENV{USER}@${CMAKE_HOSTNAME}")

 # On *nix systems, it's easier to use an external awk script
 # This way, we're not limited on the starting words of our sections (we can use random values for references)
 
 # Besides, it's possible to target only specific Markdown files
 # So it'll be easier to 'target' other entries, i.e. java files, in the future
 
 set(DOXYFILE_INPUT_FILTER "INPUT_FILTER =")
 set(DOXYFILE_FILTER_PATTERNS "FILTER_PATTERNS = \"*.md=\\\"awk -f ${MD_FILTER_OUT}\\\"\" ")

endif(WIN32)

if(VERSION_HARDCODED)

  ### FAKE info ###
  set(CMAKE_VERSION_FIRST "0")
  set(CMAKE_VERSION_MIDDLE "0")
  set(CMAKE_VERSION_LAST "0")
  set(CMAKE_GIT_DETAILS "0123456 (HEAD, origin/develop, origin/HEAD, develop) FAKE: hardcoded values without using GIT")
  set(CMAKE_GIT_COMMIT_HASH "0123456")

  ### FAKE info for CORE ###
  set(CMAKE_VERSION_FIRST_CORE "0")
  set(CMAKE_VERSION__MIDDLE_CORE "0")
  set(CMAKE_VERSION_LAST_CORE "0")
  set(CMAKE_GIT_DETAILS_CORE "6543210 (HEAD, origin/develop, origin/HEAD, develop) FAKE: hardcoded values without using GIT")
  set(CMAKE_GIT_COMMIT_HASH_CORE "6543210")

  ### FAKE info for BASE ###
  set(CMAKE_VERSION_FIRST_BASE "0")
  set(CMAKE_VERSION__MIDDLE_BASE "0")
  set(CMAKE_VERSION_LAST_CORE_BASE "0")
  set(CMAKE_GIT_DETAILS_CORE_BASE "6543210 (HEAD, origin/develop, origin/HEAD, develop) FAKE: hardcoded values without using GIT")
  set(CMAKE_GIT_COMMIT_HASH_CORE_BASE "6543210")

  ### FAKE info for UI ###
  set(CMAKE_VERSION_FIRST_UI "0")
  set(CMAKE_VERSION__MIDDLE_UI "0")
  set(CMAKE_VERSION_LAST_CORE_UI "0")
  set(CMAKE_GIT_DETAILS_CORE_UI "6543210 (HEAD, origin/develop, origin/HEAD, develop) FAKE: hardcoded values without using GIT")
  set(CMAKE_GIT_COMMIT_HASH_CORE_UI "6543210")

  ### FAKE info for PERSIST ###
  set(CMAKE_VERSION_FIRST_PERSIST "0")
  set(CMAKE_VERSION__MIDDLE_PERSIST "0")
  set(CMAKE_VERSION_LAST_CORE_PERSIST "0")
  set(CMAKE_GIT_DETAILS_CORE_PERSIST "6543210 (HEAD, origin/develop, origin/HEAD, develop) FAKE: hardcoded values without using GIT")
  set(CMAKE_GIT_COMMIT_HASH_CORE_PERSIST "6543210")

  message(STATUS "Using harcoded values")

else(VERSION_HARDCODED)

 ### Read values from global info.yaml ####
 file(READ "${INFO_YAML}" INFO)
 string(REGEX REPLACE ";" "\\\\;" INFO "${INFO}")
 string(REGEX REPLACE "\n" ";" INFO "${INFO}")
 foreach(LINE "${INFO}")

   string(REGEX MATCH "[ ]*VERSION: [0-9]*\.[0-9]*\.[0-9]*" VERSION_LINE "${LINE}")
   if(DEFINED VERSION_LINE)
    string(STRIP "${VERSION_LINE}" VERSION_LINE)
    message(STATUS "${VERSION_LINE}")
    string(REGEX REPLACE "VERSION: \([0-9]*\).*" "\\1" FIRST_DIGIT "${VERSION_LINE}")
    string(REGEX REPLACE "VERSION: [0-9]*\.\([0-9]*\).*" "\\1" MIDDLE_DIGIT "${VERSION_LINE}")
    string(REGEX REPLACE "VERSION: [0-9]*\.[0-9]*\.\([0-9]*\)" "\\1" LAST_DIGIT "${VERSION_LINE}")
   else()
    message(FATAL_ERROR "VERSION not found at ${INFO_YAML}")
   endif()

   string(REGEX MATCH "[ ]*VERSION_CORE: [0-9]*\.[0-9]*\.[0-9]*" VERSION_LINE "${LINE}")
   if(DEFINED VERSION_LINE)
    string(STRIP "${VERSION_LINE}" VERSION_LINE)
    message(STATUS "${VERSION_LINE}")
    string(REGEX REPLACE "VERSION_CORE: \([0-9]*\).*" "\\1" FIRST_DIGIT_CORE "${VERSION_LINE}")
    string(REGEX REPLACE "VERSION_CORE: [0-9]*\.\([0-9]*\).*" "\\1" MIDDLE_DIGIT_CORE "${VERSION_LINE}")
    string(REGEX REPLACE "VERSION_CORE: [0-9]*\.[0-9]*\.\([0-9]*\)" "\\1" LAST_DIGIT_CORE "${VERSION_LINE}")
   else()
    message(FATAL_ERROR "VERSION_CORE not found at ${INFO_YAML}")
   endif()

   string(REGEX MATCH "[ ]*VERSION_BASE: [0-9]*\.[0-9]*\.[0-9]*" VERSION_LINE "${LINE}")
   if(DEFINED VERSION_LINE)
    string(STRIP "${VERSION_LINE}" VERSION_LINE)
    message(STATUS "${VERSION_LINE}")
    string(REGEX REPLACE "VERSION_BASE: \([0-9]*\).*" "\\1" FIRST_DIGIT_BASE "${VERSION_LINE}")
    string(REGEX REPLACE "VERSION_BASE: [0-9]*\.\([0-9]*\).*" "\\1" MIDDLE_DIGIT_BASE "${VERSION_LINE}")
    string(REGEX REPLACE "VERSION_BASE: [0-9]*\.[0-9]*\.\([0-9]*\)" "\\1" LAST_DIGIT_BASE "${VERSION_LINE}")
   else()
    message(FATAL_ERROR "VERSION_BASE not found at ${INFO_YAML}")
   endif()

   string(REGEX MATCH "[ ]*VERSION_UI: [0-9]*\.[0-9]*\.[0-9]*" VERSION_LINE "${LINE}")
   if(DEFINED VERSION_LINE)
    string(STRIP "${VERSION_LINE}" VERSION_LINE)
    message(STATUS "${VERSION_LINE}")
    string(REGEX REPLACE "VERSION_UI: \([0-9]*\).*" "\\1" FIRST_DIGIT_UI "${VERSION_LINE}")
    string(REGEX REPLACE "VERSION_UI: [0-9]*\.\([0-9]*\).*" "\\1" MIDDLE_DIGIT_UI "${VERSION_LINE}")
    string(REGEX REPLACE "VERSION_UI: [0-9]*\.[0-9]*\.\([0-9]*\)" "\\1" LAST_DIGIT_UI "${VERSION_LINE}")
   else()
    message(FATAL_ERROR "VERSION_UI not found at ${INFO_YAML}")
   endif()

   string(REGEX MATCH "[ ]*VERSION_PERSIST: [0-9]*\.[0-9]*\.[0-9]*" VERSION_LINE "${LINE}")
   if(DEFINED VERSION_LINE)
    string(STRIP "${VERSION_LINE}" VERSION_LINE)
    message(STATUS "${VERSION_LINE}")
    string(REGEX REPLACE "VERSION_PERSIST: \([0-9]*\).*" "\\1" FIRST_DIGIT_PERSIST "${VERSION_LINE}")
    string(REGEX REPLACE "VERSION_PERSIST: [0-9]*\.\([0-9]*\).*" "\\1" MIDDLE_DIGIT_PERSIST "${VERSION_LINE}")
    string(REGEX REPLACE "VERSION_PERSIST: [0-9]*\.[0-9]*\.\([0-9]*\)" "\\1" LAST_DIGIT_PERSIST "${VERSION_LINE}")
   else()
    message(FATAL_ERROR "VERSION_PERSIST not found at ${INFO_YAML}")
   endif()

 endforeach(LINE "${INFO}")

 ### Human Info ###
 set(CMAKE_VERSION_FIRST "${FIRST_DIGIT}")
 set(CMAKE_VERSION_MIDDLE "${MIDDLE_DIGIT}")
 set(CMAKE_VERSION_LAST "${LAST_DIGIT}")

 set(CMAKE_VERSION_FIRST_CORE "${FIRST_DIGIT_CORE}")
 set(CMAKE_VERSION_MIDDLE_CORE "${MIDDLE_DIGIT_CORE}")
 set(CMAKE_VERSION_LAST_CORE "${LAST_DIGIT_CORE}")

 set(CMAKE_VERSION_FIRST_BASE "${FIRST_DIGIT_BASE}")
 set(CMAKE_VERSION_MIDDLE_BASE "${MIDDLE_DIGIT_BASE}")
 set(CMAKE_VERSION_LAST_BASE "${LAST_DIGIT_BASE}")

 set(CMAKE_VERSION_FIRST_UI "${FIRST_DIGIT_UI}")
 set(CMAKE_VERSION_MIDDLE_UI "${MIDDLE_DIGIT_UI}")
 set(CMAKE_VERSION_LAST_UI "${LAST_DIGIT_UI}")

 set(CMAKE_VERSION_FIRST_PERSIST "${FIRST_DIGIT_PERSIST}")
 set(CMAKE_VERSION_MIDDLE_PERSIST "${MIDDLE_DIGIT_PERSIST}")
 set(CMAKE_VERSION_LAST_PERSIST "${LAST_DIGIT_PERSIST}")

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

 execute_process(
  COMMAND git log -1 --format=%h 
  WORKING_DIRECTORY ${GIT_DIR_CORE} 
  OUTPUT_VARIABLE CMAKE_GIT_COMMIT_HASH_CORE
  OUTPUT_STRIP_TRAILING_WHITESPACE
 )
 execute_process(
  COMMAND git log --oneline --decorate -1
  WORKING_DIRECTORY ${GIT_DIR_CORE} 
  OUTPUT_VARIABLE CMAKE_GIT_DETAILS_CORE
  OUTPUT_STRIP_TRAILING_WHITESPACE
 )

 execute_process(
  COMMAND git log -1 --format=%h 
  WORKING_DIRECTORY ${GIT_DIR_BASE} 
  OUTPUT_VARIABLE CMAKE_GIT_COMMIT_HASH_BASE
  OUTPUT_STRIP_TRAILING_WHITESPACE
 )
 execute_process(
  COMMAND git log --oneline --decorate -1
  WORKING_DIRECTORY ${GIT_DIR_BASE} 
  OUTPUT_VARIABLE CMAKE_GIT_DETAILS_BASE
  OUTPUT_STRIP_TRAILING_WHITESPACE
 )

 execute_process(
  COMMAND git log -1 --format=%h 
  WORKING_DIRECTORY ${GIT_DIR_UI} 
  OUTPUT_VARIABLE CMAKE_GIT_COMMIT_HASH_UI
  OUTPUT_STRIP_TRAILING_WHITESPACE
 )
 execute_process(
  COMMAND git log --oneline --decorate -1
  WORKING_DIRECTORY ${GIT_DIR_UI} 
  OUTPUT_VARIABLE CMAKE_GIT_DETAILS_UI
  OUTPUT_STRIP_TRAILING_WHITESPACE
 )

 execute_process(
  COMMAND git log -1 --format=%h 
  WORKING_DIRECTORY ${GIT_DIR_PERSIST} 
  OUTPUT_VARIABLE CMAKE_GIT_COMMIT_HASH_PERSIST
  OUTPUT_STRIP_TRAILING_WHITESPACE
 )
 execute_process(
  COMMAND git log --oneline --decorate -1
  WORKING_DIRECTORY ${GIT_DIR_PERSIST} 
  OUTPUT_VARIABLE CMAKE_GIT_DETAILS_PERSIST
  OUTPUT_STRIP_TRAILING_WHITESPACE
 )

endif(VERSION_HARDCODED)

# Extra variables to be used in those templates

set(DOXYFILE_PROJECT_NUMBER "${CMAKE_VERSION_FIRST}.${CMAKE_VERSION_MIDDLE}.${CMAKE_VERSION_LAST}")
set(DOXYFILE_PROJECT_NUMBER_CORE "${CMAKE_VERSION_FIRST_CORE}.${CMAKE_VERSION_MIDDLE_CORE}.${CMAKE_VERSION_LAST_CORE}")
set(DOXYFILE_PROJECT_NUMBER_BASE "${CMAKE_VERSION_FIRST_BASE}.${CMAKE_VERSION_MIDDLE_BASE}.${CMAKE_VERSION_LAST_BASE}")
set(DOXYFILE_PROJECT_NUMBER_UI "${CMAKE_VERSION_FIRST_UI}.${CMAKE_VERSION_MIDDLE_UI}.${CMAKE_VERSION_LAST_UI}")
set(DOXYFILE_PROJECT_NUMBER_PERSIST "${CMAKE_VERSION_FIRST_PERSIST}.${CMAKE_VERSION_MIDDLE_PERSIST}.${CMAKE_VERSION_LAST_PERSIST}")
set(DOXYFILE_PROJECT_NAME "Genealogical Tree")
set(DOXYFILE_PROJECT_BRIEF "Find all the descendant on any level of ancestry")

# Folders to process
set(DOXYFILE_INPUT00 "${LOCAL_SOURCE_DIR}")
set(DOXYFILE_INPUT01 "${LOCAL_SOURCE_DIR}/doc")
set(DOXYFILE_INPUT02 "${LOCAL_SOURCE_DIR}/base/vagrant")
set(DOXYFILE_INPUT03 "${LOCAL_SOURCE_DIR}/image")
set(DOXYFILE_INPUT04 "${LOCAL_SOURCE_DIR}/template")
set(DOXYFILE_INPUT05 "${LOCAL_SOURCE_DIR}/base/test")
set(DOXYFILE_INPUT06 "${LOCAL_SOURCE_DIR}/core/optimize")
set(DOXYFILE_INPUT07 "${LOCAL_SOURCE_DIR}/base/mock")
set(DOXYFILE_INPUT08 "${LOCAL_SOURCE_DIR}/core/src")
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

## WARNING: LaTeX special characteres, as 'underscore', must be avoided
string(REGEX REPLACE "_" "-" CMAKE_GIT_DETAILS ${CMAKE_GIT_DETAILS})
string(REGEX REPLACE "&" " and " CMAKE_GIT_DETAILS ${CMAKE_GIT_DETAILS})
string(REGEX REPLACE "_" "-" GIT_SYSTEM ${GIT_SYSTEM})
string(REGEX REPLACE "&" " and " GIT_SYSTEM ${GIT_SYSTEM})
string(REGEX REPLACE "_" "-" CMAKE_GIT_DETAILS_CORE ${CMAKE_GIT_DETAILS_CORE})
string(REGEX REPLACE "&" " and " CMAKE_GIT_DETAILS_CORE ${CMAKE_GIT_DETAILS_CORE})
string(REGEX REPLACE "_" "-" CMAKE_GIT_DETAILS_BASE ${CMAKE_GIT_DETAILS_BASE})
string(REGEX REPLACE "&" " and " CMAKE_GIT_DETAILS_BASE ${CMAKE_GIT_DETAILS_BASE})
string(REGEX REPLACE "_" "-" CMAKE_GIT_DETAILS_UI ${CMAKE_GIT_DETAILS_UI})
string(REGEX REPLACE "&" " and " CMAKE_GIT_DETAILS_UI ${CMAKE_GIT_DETAILS_UI})
string(REGEX REPLACE "_" "-" CMAKE_GIT_DETAILS_PERSIST ${CMAKE_GIT_DETAILS_PERSIST})
string(REGEX REPLACE "&" " and " CMAKE_GIT_DETAILS_PERSIST ${CMAKE_GIT_DETAILS_PERSIST})

configure_file(${MD_FILTER_IN} ${MD_FILTER_OUT})
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT})
configure_file(${HEADER_IN} ${CMAKE_HEADER_OUT})

message(STATUS "GIT_DETAILS=\"${CMAKE_GIT_DETAILS}\"")
message(STATUS "GIT_SYSTEM=\"${GIT_SYSTEM}\"")

