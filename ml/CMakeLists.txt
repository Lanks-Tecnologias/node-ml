cmake_minimum_required(VERSION 3.14) # for add_link_options and implicit target directories.
project(ml C CXX)
set(GGML_VULKAN ON)
set(BUILD_SHARED_LIBS_DEFAULT ON)
#set(SD_VULKAN ON)
#set(SD_BUILD_SHARED_LIBS ON)
set(BUILD_SHARED_LIBS ON)
set(GGML_CUDA ON)
#set(SD_CUDA ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
execute_process(COMMAND node -p "require('node-addon-api').include.slice(1,-1)"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE NODE_ADDON_API_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
include_directories(${NODE_ADDON_API_DIR} ${CMAKE_JS_INC})

add_subdirectory(ggml)
#add_subdirectory(encodec)
#add_subdirectory(bark)
#add_subdirectory(whisper)
add_subdirectory(llama)
#add_subdirectory(stable-diffusion)

set(SOURCES
        addon/addon.cpp
        addon/ggml/BackendDevice.cpp
        addon/ggml/GGMLSession.cpp
        addon/ggml/Backend.cpp
        addon/ggml/GGML.cpp
        addon/ggml/BackendDevice.h
        addon/ggml/GGML.h
        addon/ggml/InitParams.cpp
        addon/ggml/InitParams.h
        addon/ggml/Context.cpp
        addon/ggml/Context.h
        addon/ggml/CGraph.cpp
        addon/ggml/CGraph.h
        addon/ggml/Tensor.cpp
        addon/ggml/Tensor.h
        addon/ggml/GraphPlan.cpp
        addon/ggml/GraphPlan.h
        addon/ggml/Backend.h
)

file(GLOB ML_SOURCES
        "*.h"
        "*.cpp"
        "*.hpp"
)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
# Nome da lib que será carregada pelo require()
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
#target_link_libraries(${PROJECT_NAME} PUBLIC
#        ggml
#        llama
#        ${SD_LIB}
#        ${ENCODEC_LIB}
#        ${BARK_LIB}
#        whisper
#)
#add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_JS_INC}
        ggml/ggml/src
        ggml/ggml/include
)
target_link_libraries(${PROJECT_NAME}
        ${CMAKE_JS_LIB}
        ggml
        llama
        ${SD_LIB}
        #whisper
       # ${BARK_LIB}
)



