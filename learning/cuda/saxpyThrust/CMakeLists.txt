project(saxpyThrust)

find_package(CUDA QUIET REQUIRED)

set(CUDA_HOST_COMPILER gcc-4.8)

set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}
  -gencode arch=compute_52,code=sm_52 -Xcompiler -std=c++98)

cuda_add_executable(saxpyThrust
  main.cu)
