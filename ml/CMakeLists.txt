cmake_minimum_required(VERSION 3.14) # for add_link_options and implicit target directories.
project("ml" C CXX)
set(GGML_VULKAN ON)
set(GGML_CUDA ON)

add_subdirectory(ggml)
add_subdirectory(encodec)
add_subdirectory(bark)
add_subdirectory(whisper)
add_subdirectory(llama)
add_subdirectory(stable-diffusion)