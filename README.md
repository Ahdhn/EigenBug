# Compilation Bug with `Eigen::SimplicialLDLT` and CUDA

[![Windows](https://github.com/Ahdhn/EigenBug/actions/workflows/Windows.yml/badge.svg)](https://github.com/Ahdhn/EigenBug/actions/workflows/Windows.yml) [![Ubuntu](https://github.com/Ahdhn/EigenBug/actions/workflows/Ubuntu.yml/badge.svg)](https://github.com/Ahdhn/EigenBug/actions/workflows/Ubuntu.yml)

This repository demonstrates a compilation bug that occurs when using `Eigen::SimplicialLDLT` with CUDA, even if the code does not execute anything on the GPU. The following code snippet:

```cpp
  Eigen::SimplicialLDLT<Eigen::SparseMatrix<float>,
                        Eigen::UpLoType::Lower,
                        Eigen::AMDOrdering<int>> solver;

solver.compute(A);
```

produces the following error in Visual Studio 2022:

```
Error: function "Eigen::Block<XprType, BlockRows, BlockCols, InnerPanel>::operator=(const Eigen::Block<Eigen::Matrix<int, -1, 1, 0, -1, 1>, -1, -1, false> &) [with XprType=Eigen::Matrix<int, -1, 1, 0, -1, 1>, BlockRows=-1, BlockCols=-1, InnerPanel=false]" (declared implicitly) cannot be referenced -- it is a deleted function
```

However, the same code compiles successfully when included in a `.cpp` file. This repository provides examples showing that the code compiles correctly with a[`.cpp` file](/EigenCPP/include/main.cpp) but fails to compile with [`.cu` file](/EigenCUDA/include/main.cu). 


The error points to [this line](https://gitlab.com/libeigen/eigen/-/blob/master/Eigen/src/Core/PlainObjectBase.h?ref_type=heads#L963) in `PlainObjectBase.h`.  It seems that the issue originates from [this line](https://gitlab.com/libeigen/eigen/-/blob/master/Eigen/src/OrderingMethods/Amd.h?ref_type=heads#L406) in the `Amd.h` file where the permutation matrix (`perm`) gets is resized.

Note that this problem does not occur if the `Eigen::COLAMDOrdering` reordering method is used.

## Build 
```
mkdir build
cd build 
cmake ..
```

Depending on the system, this will generate either a `.sln` project on Windows or a `make` file for a Linux system. 
