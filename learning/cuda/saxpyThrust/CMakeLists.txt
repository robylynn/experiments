project(saxpyThrust)

find_package(CUDA QUIET REQUIRED)

list(APPEND CUDA_NVCC_FLAGS
  "-arch=sm_52; -std=c++11")

set(CUDA_PROPAGATE_HOST_FLAGS OFF)

cuda_add_executable(saxpyThrust
  main.cu)
