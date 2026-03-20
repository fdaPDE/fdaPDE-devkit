## HPC

The folder is organized according to the compilation workflow:

- `spack/` contains examples using a Spack-based environment  
- `apptainer/` contains examples using Apptainer containers  

The Apptainer setup relies on the same Docker image used locally:
`aldoclemente/fdapde-docker:latest`.

These scripts show how to:
- submit jobs to a scheduler
- configure resources (CPUs, memory, walltime)
- compile and run your programs in a batch environment

### Usage

From the appropriate subfolder:

```bash
qsub job_script.sh
```
