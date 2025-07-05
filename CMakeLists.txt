cmake_minimum_required(VERSION 3.14) # for add_link_options and implicit target directories.
project(ml C CXX)
set(GGML_VULKAN ON)
set(BUILD_SHARED_LIBS_DEFAULT ON)
#set(SD_VULKAN ON)
#set(SD_BUILD_SHARED_LIBS ON)
set(BUILD_SHARED_LIBS ON)
set(GGML_CUDA ON)
#set(SD_CUDA ON)
set(BARK_BUILD_EXAMPLES OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
execute_process(COMMAND node -p "require('node-addon-api').include.slice(1,-1)"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE NODE_ADDON_API_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
include_directories(${NODE_ADDON_API_DIR}
        ${CMAKE_JS_INC}
        ml/ggml/ggml
        ml/ggml/ggml/src
        ml/bark/bark.cpp
        ml/encodec/encodec.cpp
        ml/llama/llama.cpp/include
        ml/stable-diffusion/stable-diffusion.cpp
        ml/stable-diffusion/stable-diffusion.cpp/thirdparty
)

add_subdirectory(ml/ggml)
add_subdirectory(ml/encodec)
add_subdirectory(ml/bark)
add_subdirectory(ml/whisper)
add_subdirectory(ml/llama)
add_subdirectory(ml/stable-diffusion)

set(SOURCES
        ml/addon/addon.cpp
        ml/addon/ggml/Device.cpp
        ml/addon/ggml/GGMLSession.cpp
        ml/addon/ggml/Backend.cpp
        ml/addon/ggml/GGML.cpp
        ml/addon/ggml/Device.h
        ml/addon/ggml/GGML.h
        ml/addon/ggml/InitParams.cpp
        ml/addon/ggml/InitParams.h
        ml/addon/ggml/Context.cpp
        ml/addon/ggml/Context.h
        ml/addon/ggml/CGraph.cpp
        ml/addon/ggml/CGraph.h
        ml/addon/ggml/Tensor.cpp
        ml/addon/ggml/Tensor.h
        ml/addon/ggml/GraphPlan.cpp
        ml/addon/ggml/GraphPlan.h
        ml/addon/ggml/Backend.h
        ml/addon/ggml/BackendReg.cpp
        ml/addon/ggml/BackendReg.h
        ml/addon/ggml/Scheduler.cpp
        ml/addon/ggml/Scheduler.h
        ml/addon/ggml/BufferType.cpp
        ml/addon/ggml/BufferType.h
        ml/addon/ggml/Buffer.cpp
        ml/addon/ggml/Buffer.h
        ml/addon/ggml/Plan.cpp
        ml/addon/ggml/Plan.h
        ml/addon/ggml/GGUF.cpp
        ml/addon/ggml/GGUF.h
        ml/addon/Llama.cpp
        ml/addon/Llama.h
        ml/addon/StableDiffusion.cpp
        ml/addon/StableDiffusion.h
        ml/addon/Bark.cpp
        ml/addon/Bark.h
        ml/addon/Encodec.cpp
        ml/addon/Encodec.h
        ml/addon/Whisper.cpp
        ml/addon/Whisper.h
        ml/addon/Upscaler.cpp
        ml/addon/Upscaler.h
        ml/addon/thirdparty/dr_wav.h
)

file(GLOB ML_SOURCES
        "ml/*.h"
        "ml/*.cpp"
        "ml/*.hpp"
)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
# Nome da lib que será carregada pelo require()
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
#target_link_libraries(${PROJECT_NAME} PUBLIC
##        ggml
##        llama
##        ${SD_LIB}
#        ${ENCODEC_LIB}
#        ${BARK_LIB}
#        whisper
#)
#add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_JS_INC}
        ml/ggml/ggml/src
        ml/ggml/ggml/include
        ml/encodec/encodec.cpp
        ml/bark/bark.cpp
        ml/llama/llama.cpp/include
        ml/stable-diffusion/stable-diffusion.cpp
        ml/stable-diffusion/stable-diffusion.cpp/thirdparty
)
target_link_libraries(${PROJECT_NAME}
        ${CMAKE_JS_LIB}
        ggml
        encodec
        whisper
        bark
        stable-diffusion
        llama
)