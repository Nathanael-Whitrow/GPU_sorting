#!/bin/bash
# Configure the resources required
#SBATCH -p test             # partition queue your job will be added to
#SBATCH -n 1                # number of cores
#SBATCH --time=00:05:00     # time allocation, which has the format DD:HH:MM
#SBATCH --gres=gpu:1        # generic resource required (1 GPU)
#SBATCH --mem=16GB          # memory pool for all cores

module load CUDA
make
./main
