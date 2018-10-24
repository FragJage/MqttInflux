#Activer le support c++11 
if(CMAKE_VERSION VERSION_LESS "3.1")
  set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
else ()
  set(CMAKE_CXX_STANDARD 11)
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated") # various warning flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -O2") # no debug, active optimisation

find_package(CURL REQUIRED) 

include_directories(${CURL_INCLUDE_DIR})
include_directories(../dependencies/cpr/include)
include_directories(../dependencies/FragFramework/src)
include_directories(../dependencies/MqttDaemon/src)
include_directories(../dependencies/ServiceSkeleton/src)
include_directories(../dependencies/SimpleFolder/src)
include_directories(../dependencies/SimpleIni/include)
include_directories(../dependencies/SimpleLog/src)
 
#Génération de la liste des fichiers sources
file(
  GLOB_RECURSE
  Cpr_files
  ../dependencies/cpr/cpr/*
)

list(REMOVE_ITEM
  Cpr_files
  "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/cpr/cpr/multipart.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/cpr/cpr/parameters.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/cpr/cpr/payload.cpp"
)

set(
  FragFramework_files
  "../dependencies/FragFramework/src/Plateforms.cpp"
)

set(
  MqttDaemon_files
  "../dependencies/MqttDaemon/src/MqttBase.cpp"
  "../dependencies/MqttDaemon/src/MqttDaemon.cpp"
)

file(
  GLOB_RECURSE
  ServiceSkeleton_files
  ../dependencies/ServiceSkeleton/src/*
)

file(
  GLOB_RECURSE
  SimpleFolder_files
  ../dependencies/SimpleFolder/src/*
)

file(
  GLOB_RECURSE
  SimpleIni_files
  ../dependencies/SimpleIni/src/*
)

file(
  GLOB_RECURSE
  SimpleLog_files
  ../dependencies/SimpleLog/src/*
)

#Déclaration de l'exécutable
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
add_executable(
  MqttInflux
  ${Cpr_files}
  ${FragFramework_files}
  ${MqttDaemon_files}
  ${ServiceSkeleton_files}
  ${SimpleFolder_files}
  ${SimpleIni_files}
  ${SimpleLog_files}
  MqttBridge.cpp
  MqttInflux.cpp
  main.cpp
)

target_link_libraries(MqttInflux paho-mqtt3a)
target_link_libraries(MqttInflux paho-mqttpp3)
target_link_libraries(MqttInflux ${CURL_LIBRARIES})

#Spécification WIN32
if(WIN32)
  target_link_libraries(MqttInflux ws2_32)
endif()
