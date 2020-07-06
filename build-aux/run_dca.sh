#!/bin/bash
#SBATCH --job-name=dca_trail
#SBATCH --output=slurm.%j.out
#SBATCH --error=slurm.%j.err
#SBATCH --nodes=2
#SBATCH --ntasks-per-core=1
#SBATCH --ntasks-per-node=2
#SBATCH --cpus-per-task=6
#SBATCH --time=00:05:00
#SBATCH --exclusive
#SBATCH --distribution=cyclic
#SBATCH --constraint=gpu
#SBATCH --partition=normal

source ~/dev/src/master-dca/DCA/build-aux/daint_load_modules.sh 

cd /users/wweile/dev/src/master-dca/DCA/build/applications/dca

srun -n $SLURM_NTASKS --ntasks-per-core=$SLURM_NTASKS_PER_CORE --ntasks-per-node=$SLURM_NTASKS_PER_NODE -c $SLURM_CPUS_PER_TASK ./main_dca input_small_run_benchmark_1nodes.json 

#srun ./main_dca input_small_run_benchmark_1nodes.json
