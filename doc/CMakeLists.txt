cmake_minimum_required(VERSION 3.1)


 if(APPLE)

  set(EXTRA_ARG "PATH=\"$ENV{PATH}:/usr/texbin\"") # pdflatex
  ### make
  if(${CMAKE_BUILD_TOOL} MATCHES ".*make$")
    message(STATUS "Using defined make")
  else(${CMAKE_BUILD_TOOL} MATCHES ".*make$")
    set(CMAKE_BUILD_TOOL "/usr/bin/make")
  endif(${CMAKE_BUILD_TOOL} MATCHES ".*make$")
  message(STATUS "CMAKE_BUILD_TOOL = ${CMAKE_BUILD_TOOL}")

 elseif(UNIX)

  if(EXISTS /c/Windows)
     message(STATUS "Using Linux-like configuration")
  endif(EXISTS /c/Windows)

  ### make
  if(${CMAKE_BUILD_TOOL} MATCHES ".*make$")
     message(STATUS "Using defined make")
  else(${CMAKE_BUILD_TOOL} MATCHES ".*make$")
     set(CMAKE_BUILD_TOOL "/usr/bin/make")
  endif(${CMAKE_BUILD_TOOL} MATCHES ".*make$")
  message(STATUS "CMAKE_BUILD_TOOL = ${CMAKE_BUILD_TOOL}")

 elseif(WIN32)

  ### make
  if(${CMAKE_BUILD_TOOL} MATCHES ".*make.*$")
    message(STATUS "Using defined make")
  else(${CMAKE_BUILD_TOOL} MATCHES ".*make.*$")
    file(TO_CMAKE_PATH "C:/msys64/usr/bin/make" CMAKE_BUILD_TOOL)
  endif(${CMAKE_BUILD_TOOL} MATCHES ".*make.*$")
  message(STATUS "CMAKE_BUILD_TOOL = ${CMAKE_BUILD_TOOL}")

 endif(APPLE)


# Useful commands 
set(INVOKE_CMAKE_SCRIPT ${CMAKE_COMMAND} -DLOCAL_SOURCE_DIR="${CMAKE_SOURCE_DIR}" -P) 
set(INVOKE_CMAKE_SCRIPT_HARDCODED ${CMAKE_COMMAND} -DLOCAL_SOURCE_DIR="${CMAKE_SOURCE_DIR}" -DVERSION_HARDCODED=TRUE -P) 
set(COPY_DIFF ${CMAKE_COMMAND} -E copy_if_different) 
set(COPY_DIR ${CMAKE_COMMAND} -E copy_directory) 
set(REMOVE_DIR ${CMAKE_COMMAND} -E remove_directory)
set(REMOVE ${CMAKE_COMMAND} -E remove -f)
set(ENV_CMD ${CMAKE_COMMAND} -E env ${EXTRA_ARG})

#### Git Info ####
add_subdirectory(${PROJECT_SOURCE_DIR}/template ${PROJECT_BINARY_DIR}/doc_template)

# clean & regenerate git commit hash
add_custom_target(clean_git_commit_hash ${REMOVE} ${PROJECT_BINARY_DIR}/Doxyfile
   COMMAND ${REMOVE} ${PROJECT_BINARY_DIR}/header.tex
   COMMAND ${REMOVE_DIR} ${PROJECT_BINARY_DIR}/latex
   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)
 
if(VERSION_HARDCODED)
  add_custom_target(git_commit_hash ${INVOKE_CMAKE_SCRIPT_HARDCODED} ${PROJECT_SOURCE_DIR}/template/CMakeLists.txt 
    DEPENDS clean_git_commit_hash 
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
else(VERSION_HARDCODED)
  add_custom_target(git_commit_hash ${INVOKE_CMAKE_SCRIPT} ${PROJECT_SOURCE_DIR}/template/CMakeLists.txt
    DEPENDS clean_git_commit_hash 
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
endif(VERSION_HARDCODED)

############# DOCUMENTATION ##############

find_package(Doxygen)
if(DOXYGEN_FOUND)

   # TARGET
   set(TMP_PDF_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.pdf)
   set(TMP_HTML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/html)

   if(${DOC_PDF} MATCHES ".*pdf$")
     set(PDF_FILE "${DOC_PDF}")
   else(${DOC_PDF} MATCHES ".*pdf$")

     if(WIN32)
           file(TO_CMAKE_PATH "C:/Users/Public/${CMAKE_PROJECT_NAME}.pdf" PDF_FILE)
     else(WIN32)
           if(EXISTS /c/Windows)
               set(PDF_FILE /c/Users/Public/${CMAKE_PROJECT_NAME}.pdf)
           else(EXISTS /c/Windows)
               set(PDF_FILE $ENV{HOME}/${CMAKE_PROJECT_NAME}.pdf)
           endif(EXISTS /c/Windows)
     endif(WIN32)

   endif(${DOC_PDF} MATCHES ".*pdf$")

   if(${DOC_HTML} MATCHES "..*$")
     set(HTML_DIR "${DOC_HTML}")
   else(${DOC_HTML} MATCHES "..*$")
     if(WIN32)
       file(TO_CMAKE_PATH "C:/Users/Public/${CMAKE_PROJECT_NAME}_html" HTML_DIR)
     else(WIN32)
       if(EXISTS /c/Windows)
         set(HTML_DIR /c/Users/Public/${CMAKE_PROJECT_NAME}_html)
       else(EXISTS /c/Windows)
         set(HTML_DIR $ENV{HOME}/${CMAKE_PROJECT_NAME})
       endif(EXISTS /c/Windows)
     endif(WIN32)
   endif(${DOC_HTML} MATCHES "..*$")

  if(APPLE)

     ### PDF Reader 
     if($ENV{PDF_READER} MATCHES "..*")
       set(PDF_READER "$ENV{PDF_READER}")
     else($ENV{PDF_READER} MATCHES "..*")
       set(PDF_READER "Preview")
     endif($ENV{PDF_READER} MATCHES "..*")
    
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

     if(EXISTS /c/Windows)

       ### PDF Reader 
       if($ENV{PDF_READER} MATCHES ".*exe$")
         file(TO_CMAKE_PATH "$ENV{PDF_READER}" PDF_READER_CMAKE)
       else($ENV{PDF_READER} MATCHES ".*exe$")
         file(TO_CMAKE_PATH "/c/Program\ Files (x86)/Adobe/Reader\ 11.0/Reader/AcroRd32.exe" PDF_READER_CMAKE)
       endif($ENV{PDF_READER} MATCHES ".*exe$")
       set(PDF_READER "${PDF_READER_CMAKE}")

    
       ### Utility to show pdfs
       set(UTILITY  "$ENV{USERPROFILE}/show.bat") 
       file(WRITE ${UTILITY} "@echo off\n")
       file(APPEND ${UTILITY} "echo Generating documentation ...\n")
       file(APPEND ${UTILITY} "make doc -f ${PROJECT_BINARY_DIR}/Makefile -C ${PROJECT_BINARY_DIR} > NUL 2>&1\n")
       file(APPEND ${UTILITY} "echo Documentation generated\n")
       file(APPEND ${UTILITY} "start /I \"${PDF_READER}\" \"${PDF_FILE}\"\n")
       message(STATUS "Created ~/show utility")

     else(EXISTS /c/Windows)
 
       ### PDF Reader 
       if($ENV{PDF_READER} MATCHES "..*")
         set(PDF_READER "$ENV{PDF_READER}")
       else($ENV{PDF_READER} MATCHES "..*")
         set(PDF_READER "evince")
       endif($ENV{PDF_READER} MATCHES "..*")
 
       ### Utility to show pdfs
       set(UTILITY  "${PROJECT_BINARY_DIR}/show") 
       file(WRITE ${UTILITY} "#!/bin/bash\n")
       file(APPEND ${UTILITY} "make show -f ${PROJECT_BINARY_DIR}/Makefile -C ${PROJECT_BINARY_DIR} > /dev/null 2>&1\n")
       file(COPY ${UTILITY} DESTINATION $ENV{HOME} 
            FILE_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE
                             GROUP_WRITE GROUP_READ GROUP_EXECUTE
                             WORLD_EXECUTE)
       message(STATUS "Created ~/show utility")

     endif(EXISTS /c/Windows)

   elseif(WIN32)

     ### PDF Reader 
     if($ENV{PDF_READER} MATCHES ".*exe$")
       file(TO_CMAKE_PATH "$ENV{PDF_READER}" PDF_READER_CMAKE)
     else($ENV{PDF_READER} MATCHES ".*exe$")
       file(TO_CMAKE_PATH "C:\\Program Files (x86)\\Adobe\\Reader 11.0\\Reader\\AcroRd32.exe" PDF_READER_CMAKE)
     endif($ENV{PDF_READER} MATCHES ".*exe$")
     set(PDF_READER "${PDF_READER_CMAKE}")
    
     ### Utility to show pdfs
     set(UTILITY  "$ENV{USERPROFILE}/show.bat") 
     file(WRITE ${UTILITY} "@echo off\n")
     file(APPEND ${UTILITY} "echo Generating documentation ...\n")
     file(APPEND ${UTILITY} "make doc -f ${PROJECT_BINARY_DIR}/Makefile -C ${PROJECT_BINARY_DIR} > NUL 2>&1\n")
     file(APPEND ${UTILITY} "echo Documentation generated\n")
     file(APPEND ${UTILITY} "start /I \"${PDF_READER}\" \"${PDF_FILE}\"\n")
     message(STATUS "Created ~/show utility")

 endif(APPLE)

 # make image
 add_custom_target( image ${INVOKE_CMAKE_SCRIPT} ${PROJECT_SOURCE_DIR}/image/CMakeLists.txt
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
 )

 # make doc --> ALL
 add_custom_target( doc ALL ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
   COMMAND ${ENV_CMD} ${CMAKE_BUILD_TOOL} -Wno-dev -f ${PROJECT_BINARY_DIR}/latex/Makefile -C ${PROJECT_BINARY_DIR}/latex
   COMMAND ${COPY_DIFF} ${PROJECT_BINARY_DIR}/latex/refman.pdf ${TMP_PDF_FILE}
   COMMAND ${COPY_DIR} ${PROJECT_BINARY_DIR}/html ${TMP_HTML_DIR}
   DEPENDS image
   DEPENDS git_commit_hash 
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
 )

 # make install_doc
 add_custom_target( install_doc echo "TMP_PDF_FILE=${TMP_PDF_FILE}" 
   COMMAND echo "PDF_FILE=${PDF_FILE}"
   COMMAND echo "HTML_DIR=${HTML_DIR}"
   COMMAND ${COPY_DIFF} ${TMP_PDF_FILE} ${PDF_FILE} 
   COMMAND ${COPY_DIR} ${TMP_HTML_DIR} ${HTML_DIR} 
   DEPENDS doc
 )

 # make show
 if(APPLE)
   add_custom_target( show open -a ${PDF_READER} ${PDF_FILE} DEPENDS install_doc )
 elseif(UNIX)
   if(EXISTS /c/Windows)
     message(STATUS "Not fully support for \"make show\" on Windows") # due to error messages after closing that PDF 
     add_custom_target( show echo run ~/show for Powershell or %USERPROFILE%/show for MSDOS DEPENDS install_doc )
   else(EXISTS /c/Windows)
     add_custom_target( show ${PDF_READER} ${PDF_FILE} DEPENDS install_doc )
   endif(EXISTS /c/Windows)
 elseif(WIN32)
   message(STATUS "Not fully support for \"make show\" on Windows") # due to error messages after closing that PDF 
   add_custom_target( show echo "run ~/show (Powershell) or %USERPROFILE%/show (MSDOS)" DEPENDS install_doc )
 endif(APPLE)
	
endif(DOXYGEN_FOUND)
