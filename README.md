# Parallelizing Quadtree Image Decomposition with mixed approaches

Authors: [Gabriele Masina](https://github.com/masinag) and [Andrea Stedile](https://github.com/andreastedile)

Project required for the fulfillment of the Advanced Computing Architectures course
at University of Trento, taught by professor R. Passerone in the 2021/2022 A.Y. We implemented and evaluated various
approaches to the parallelization of the Quadtree image decomposition algorithm, such as CPU-based, CUDA-based and
mixed approaches.

**This repository implements the CPU approaches.**

## Compile

Locally:

```shell
mkdir build && cd build
conan install --build=missing ..
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

On the University of Trento's HPC cluster, do this instead:

```shell
module load gcc91
module load cuda-11.1
mkdir build && cd build
conan install --build=missing ..
/apps/cmake-3.20.3/bin -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-hpc.cmake -DCMAKE_BUILD_TYPE=Release ..
/apps/cmake-3.20.3/bin --build .
```

## Usage

```shell
Usage: app [options] 

Optional arguments:
-h --help          	shows help message and exits [default: false]
-v --version       	prints version information and exits [default: false]
--input            	specify the input file [required]
--top-down         	specify whether to execute the top-down algorithm instead of the default bottom-up one [default: false]
--detail-threshold 	specify the detail threshold [default: 13]
--no-output-file   	suppress the production of the resulting image [default: false]
```

![Demo](https://github.com/andreastedile/aca/blob/master/demo/result.gif)
