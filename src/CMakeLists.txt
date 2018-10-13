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
include_directories(../dependency)
include_directories(../dependency/cpr/include)
 
#Génération de la liste des fichiers sources
file(
  GLOB_RECURSE
  Cpr_files
  ../dependency/cpr/cpr/*
)

file(
  GLOB_RECURSE
  MqttBase_files
  ../dependency/MqttBase/*
)

file(
  GLOB_RECURSE
  Service_files
  ../dependency/Service/*
)

file(
  GLOB_RECURSE
  SimpleFolders_files
  ../dependency/SimpleFolders/*
)

file(
  GLOB_RECURSE
  SimpleIni_files
  ../dependency/SimpleIni/*
)

file(
  GLOB_RECURSE
  SimpleLog_files
  ../dependency/SimpleLog/*
)

#Déclaration de l'exécutable
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
add_executable(
  MqttInflux
  ${Cpr_files}
  ${MqttBase_files}
  ${Service_files}
  ${SimpleFolders_files}
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
