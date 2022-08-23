#!/bin/sh
### Set the job name (for your reference)
#PBS -N fpt_5
### Set the project name, your department code by default
#PBS -P cse
### Request email when job begins and ends, don't change anything on the below line 
### #PBS -m bea
### Specify email address to use for notification, don't change anything on the below line
### #PBS -M $USER@iitd.ac.in
#### Request your resources, just change the numbers
#PBS -l select=1:ncpus=1:ngpus=0:mem=16G
### Specify "wallclock time" required for this job, hhh:mm:ss
#PBS -l walltime=010:00:00

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

# module load apps/anaconda/3
module load compiler/gcc/7.1.0/compilervars

make fptree
time ./fpt ./webdocs.dat 5 ./webdocs_fp_5.dat
