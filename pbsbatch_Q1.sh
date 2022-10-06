#!/bin/sh
### Set the job name (for your reference)
#PBS -N Q1_Graphs
### Set the project name, your department code by default
#PBS -P cse
### Request email when job begins and ends, don't change anything on the below line 
#PBS -m bea
### Specify email address to use for notification, don't change anything on the below line
#PBS -M $USER@iitd.ac.in
#### Request your resources, just change the numbers
#PBS -l select=1:ncpus=1:ngpus=1:mem=16G
### Specify "wallclock time" required for this job, hhh:mm:ss
#PBS -l walltime=05:00:00
#PBS -l software=PYTHON

# After job starts, must goto working directory. 
# $PBS_O_WORKDIR is the directory from where the job is fired. 
echo "==============================="
echo $PBS_JOBID
cat $PBS_NODEFILE
echo "==============================="
cd $PBS_O_WORKDIR
echo $PBS_O_WORKDIR

module load apps/anaconda/3 

python3 Q1/plot.py Q1/formatted.txt Q1/output_plot.png Q1/totGraphs.txt