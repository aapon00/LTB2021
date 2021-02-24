# What is this?
Digital Plant is a project to simulate an industrial plant's IoT solution. It contains code (in C++ and Python) to simulate a sensor generating data. Thus, the code is a "synthetic data generator." This project also contains bash scripts to run multiple "sensors" generating synthetic data (i.e. multiple processes of the program). Furthermore, since we used Clemson's Palmetto HPC cluster to run our experiments, the project includes pbs scripts to run compute jobs on Palmetto. We also store results in this repository, although just the log files and not the latency files.
# How do I use this?
Our recommendation is to use the C++ version, since the Python version is deprecated. The Python version does not log latencies or have a bash script for running multiple processes simultaneously. It was kept as a model when building the C++ code since Brandon Posey wrote it to mimic real sensors. We presume they are sensors used by BMW, but we have not verified this. More information on the Python version can be found in the *Why are there old, irrelevant folders?* section.
## Environment
We have successfully run our code on a local machine, on Palmetto, and on an AWS t2.micro server instance. Since Palmetto does not have root access, we had to implement various workarounds such as using Spack to get the C++ code working there. The C++ code itself should be portable given a compiler supporting the C++11 standard and installation of the Boost library. However, the CMakeLists file will need modification in order to include the Azure IoT SDK C library.
### Palmetto
Palmetto is Clemson University's High Performance Computing cluster. Our aim was to take advantage of multiple nodes in order to run tens of thousands of sensors simultaneously. However, we have not achieved this yet and run only on a single node for now. We have run up to 4,000 sensors on a single node, however.
#### Qsub and PBS
Qsub is a job queueing service on Palmetto that facilitates running a job on one or more nodes. Qsub allows you to specify the compute resources you need and bash commands to run in a PBS file. We have two PBS files for qsub jobs:
1. `one_node_experiment.pbs` runs an experiment i.e. run a certain number of sensors with X, Y, ... parameters
2. `jupyter.pbs` opens a Jupyter notebook on Palmetto to create visualizations
3. `three_tier_experiment.pbs` sends messages to three tier levels to probe IotHub middleware limits 
#### Spack

### C++
The C++ code uses C++11 standard. We use CMake to build our program, which is required to incorporate the Azure IoT SDK C library. We also use Boost to implement a random number generator producing samples which follow a Pareto distribution (thanks to Robert Underwood for writing that for us!).
#### Compiling the C++ version on Palmetto
1. Get the right kind of node
```bash
qsub -I -l select=1:ncpus=16:chip_type=e5-2665:interconnect=fdr:mem=60gb,walltime=01:00:00,place=pack
```
2. Commands to run once you're on the node
```bash
module add gcc/8.2.0
source /zfs/dicelab/whalabi/spack/share/spack/setup-env.sh
spack load azureiot@LTS_05_2020_Ref01
spack load cmake
export IOT_HOME=`spack location -i azureiot@LTS_05_2020_Ref01`
```
3. Compile the code
```
cd C_Synthetic_Data_Generator
mkdir cmake
cd cmake
cmake ..
make
# or equivalently cmake --build .
```
#### Running Experiments with the C++ version
Running an experiment is as simple as running a job using qsub on Palmetto. The code will be executed and the results copied to the **results** folder automatically. The steps are as follows:
1. Make sure you have created the following two files inside the **bash_scripts** folder
    1. parameters.list
	2. connection_strings.list (currently, we use a file called connection_strings_10000.list on Palmetto)
2. If the parameters.list file contains all the parameters you want for your experiment, then skip this step. Otherwise, edit that file according to your needs.
3. run `qsub one_node_experiment.pbs` inside the `pbs/` folder or otherwise use a qualified path to the file. That's it!
# Where do I find the code for visualizations?
analysis
# Why are there old, irrelevant folders?
This is a research project, so over time we inevitably move from one direction to another. Sometimes old work is worth keeping; sometimes it is not. We have left in the older work we think was worth keeping.
- We left the Python version of the code as a reference for ourselves and for future researchers. We stopped working on it with the assumption that a C++ version would require a smaller memory footprint than the Python version. We also considered that we might need multithreading capabilities, something C++ handles more readily than Python.
# Requirements
### C++
- C++11
- CMake 3.5.1 or higher (we use 3.16.0 and have been successful with 3.13.1)
- GCC v8.2.0 (we have also been successful with 5.4.0 on AWS).
- Boost library for C++
    - Boost::math_c99
    - Boost::math_c99f
    - Boost::math_c99l
    - Boost::math_tr1
    - Boost::math_tr1f
    - Boost::math_tr1
### Python
- python 3.5 or higher
- The Azure Python IoT SDK, installed using `pip install azure-iot-device`
