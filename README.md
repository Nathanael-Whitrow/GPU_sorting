# GPU_sorting
Bitonic sort for GPU implemented in OpenCL.

Written by Nathanael Whitrow.
For comments, questions and feedback please drop me a note at:
nathanael.whitrow@gmail.com

Sorts an array of integers
 - kernel.cl is the OpenCL C code that is compiled and run on the target compute unit
 - parallel_bitonic_sort.cpp is the source code that is run on the Host system

Edit the parallel_bitonic_sort.cpp file as necessarry.
Touch the kernel.cl at your own risk.

## Compiling and Running
Your system will need:
- A recent c++ compiler (such as gcc 11)
- the [OpenCL](https://www.khronos.org/opencl/) libraries installed
Then compile the parallel_bitonic_sort.cpp file, linking to OpenCL, and run the resulting executable.

Example:
My system is an intel 11700kf CPU and nVidia 3090 GPU.
I'm running Manjaro linux LTS kernel with nVidia 495.44 drivers and CUDA 11.5. (natively, no virtualization).
On my system OpenCL libraries are installed in `/opt/cuda/targets/x86_64-linux/include/CL/`
and I had to create symlinks to them in /usr/lib64 so the linker can find them.
Then I can compile with:
```
g++ --std=c++20 parallel_bitonic_sort.cpp -o gpu_sort -lOpenCL 
```
and run with:
```
./gpu_sort
```

Typical output looks like this:
```
List creation time: 0 seconds.
Kernel compile time: 0 seconds.
local work group size: 1024
Setup time: 0 seconds.
Sorting time: 0 seconds.
max item size: 524288
checking...
done checking.
```
