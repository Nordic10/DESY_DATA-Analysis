#!/bin/bash

#SBATCH --time=10:00:00
#SBATCH -A phys56xx
#SBATCH --output=slurm.log
#SBATCH --partition=standard

source ~/.bashrc
56xx

bash cpp.sh

echo "finished"
