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
 set(IMAGE_DIR "${LOCAL_SOURCE_DIR}/image")
 set(ROOT_DIR "${LOCAL_SOURCE_DIR}")
else()
 set(IMAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
 set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
endif()

set(BUILD_IMAGE_DIR "${CMAKE_BINARY_DIR}/image")

if(APPLE)

     ### PlantUML
     if($ENV{PLANTUMLJAR} MATCHES ".*jar$")
       set(PLANTUML java -jar $ENV{PLANTUMLJAR} -v -o ${BUILD_IMAGE_DIR})
     else()
       set(PLANTUML java -jar /opt/plantuml/plantuml.jar -v -o ${BUILD_IMAGE_DIR})
     endif()

elseif(UNIX)

     ### PlantUML
     if($ENV{PLANTUMLJAR} MATCHES ".*jar$")
       set(PLANTUML java -jar $ENV{PLANTUMLJAR} -v -o ${BUILD_IMAGE_DIR})
     else()
       set(PLANTUML java -jar /opt/plantuml/plantuml.jar -v -o ${BUILD_IMAGE_DIR})
     endif()

elseif(WIN32)

     ### PlantUML
     if($ENV{PLANTUMLJAR} MATCHES ".*jar$")
      file(TO_CMAKE_PATH "$ENV{PLANTUMLJAR}" PLANTUMLJAR_CMAKE)
      set(PLANTUML java -jar ${PLANTUMLJAR_CMAKE} -v -o ${BUILD_IMAGE_DIR})
     else()
      set(PLANTUML java -jar C:/strawberry/perl/site/bin/plantuml.jar -v -o ${BUILD_IMAGE_DIR})
     endif()
 
endif()
 
#### invoke PlantUML ####
execute_process(
  COMMAND ${PLANTUML} README.md src/*.* optimize/*.* test/*.* template/*.*
  WORKING_DIRECTORY ${ROOT_DIR} 
)

#### list of images ####
file(GLOB IMAGE_LIST RELATIVE ${BUILD_IMAGE_DIR} ${BUILD_IMAGE_DIR}/*.png)

#### if new ###
#### TODO: compare metadata ####
foreach(PNG ${IMAGE_LIST})
  execute_process(
   COMMAND ${COPY_DIFF} ${BUILD_IMAGE_DIR}/${PNG} ${IMAGE_DIR}/${PNG}
   WORKING_DIRECTORY ${CMAKE_BINARY_DIR} 
  )
endforeach()
