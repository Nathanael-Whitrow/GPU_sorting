# GPU Sorting: Bitonic Sort in OpenCL

This repository contains an implementation of the Bitonic sorting algorithm for GPUs, written in OpenCL. The code was developed by me, Nathanael Whitrow. If you have comments, questions, or feedback, you can reach me at 'nathanael.whitrow@gmail.com'.

## Contents

The repository consists of two main files:

1. `kernel.cl`: This is the OpenCL C code that gets compiled and run on the target compute unit.
2. `parallel_bitonic_sort.cpp`: This is the source code that gets run on the Host system.

While you can edit the `parallel_bitonic_sort.cpp` file as necessary, please only modify `kernel.cl` if you're confident in your ability to do so safely.

## Requirements

To compile and run the program, you'll need:

- A recent C++ compiler (such as GCC 11)
- The [OpenCL](https://www.khronos.org/opencl/) libraries installed

## Compilation and Execution

Compile the `parallel_bitonic_sort.cpp` file, linking to OpenCL, and run the resulting executable. For example:

```bash
g++ --std=c++20 parallel_bitonic_sort.cpp -o gpu_sort -lOpenCL 
./gpu_sort
```

## Troubleshooting

If you're running on a system similar to mine (an Intel 11700kf CPU, nVidia 3090 GPU, Manjaro Linux LTS kernel with nVidia 495.44 drivers, and CUDA 11.5), you might need to create symlinks to your OpenCL libraries in `/usr/lib64` so the linker can find them. I have them installed in `/opt/cuda/targets/x86_64-linux/include/CL/`.

## Output

When run, the program should produce output similar to the following:

```bash
List creation time: 0 seconds.
Kernel compile time: 0 seconds.
local work group size: 1024
Setup time: 0 seconds.
Sorting time: 0 seconds.
max item size: 524288
checking...
done checking.
```

## Contributing

I appreciate contributions from users! If you want to contribute, follow these steps:

1. Fork the repository.
2. Create a branch for your new feature or bug fix.
3. Commit your changes.
4. Push to the branch on your fork.
5. Submit a pull request to the main repository.

I don't have guidelines or a code of conduct, please just be a good human.

## License

This code is free in every sense - it is not under any licence.

## Contact

If you have any questions about this repository, feel free to contact me at nathanael.whitrow@gmail.com.
