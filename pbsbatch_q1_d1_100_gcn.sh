#!/bin/sh
### Set the job name (for your reference)
#PBS -N q1_d1_100_gcn
### Set the project name, your department code by default
#PBS -P cse
### Request email when job begins and ends, don't change anything on the below line
### Specify email address to use for notification, don't change anything on the below line
#### Request your resources, just change the numbers
#PBS -l select=1:ncpus=4:ngpus=0:mem=32G
### Specify "wallclock time" required for this job, hhh:mm:ss
#PBS -l walltime=010:00:00
#PBS -l software=PYTHON

# After job starts, must goto working directory. 
# $PBS_O_WORKDIR is the directory from where the job is fired. 
echo "==============================="
echo $PBS_JOBID
cat $PBS_NODEFILE
echo "==============================="
cd $PBS_O_WORKDIR
echo $PBS_O_WORKDIR


module () {
        eval `/usr/share/Modules/3.2.10/bin/modulecmd bash $*`
}
#module load apps/anaconda/3EnvCreation
#conda activate ../.env
#conda env update --file A3/environment.yml --prune
module load apps/anaconda/3
source activate gnn
module purge
cd Q1
python main.py ../a3_datasets/d1_X.csv ../a3_datasets/conv_d1_adj_mx.csv ../a3_datasets/d1_graph_splits.npz d1 100 gcn
