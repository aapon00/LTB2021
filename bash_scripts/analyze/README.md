# Analyzing Produced logs
Bash scripts parse and and concatanate produced csv logs. 

### concat_csv.sh
/path/concat_csv.sh <log_directory>

example: concat_csv.sh /zfs/dicelab/DigitalPlant/results/May_12_24_37_37_node1660/logs/

example: /zfs/dicelab/DigitalPlant/bash_scripts/analyze/concat_csv.sh logs

### concat_all_tier.sh
/path/concat_all_tier.sh <directory containting tier run folders>

Use this to create concatanted csv for experiments that were created by the three_tier pbs script.

# Jupyter
To connect to a jupyter notebook, you will need 2 terminals. For ease of readability, I will use port 8889 throughtout the example, but you are free to choose any available port number. 

### Initial Setup
First you need to create your conda enviornment to run the jupyter notebooks

First Log into Palmetto and request an interactive job
```
$ module load anaconda3/5.1.0
$ conda env create -f /zfs/dicelab/DigitalPlant/bash_scripts/analyze/environment.yml
$ source activate dicelab
```

###Connecting to Jupyter
- local$ refers to commands that are run on your local computer
- login$ refers to commands thar are run on the login node
- nodexxx$ refers to commands that are run on an interactive job

1. local$ ssh login.palmetto.clemson.edu
2. login$ qsub -I
    - example: qsub -I -l select=1:ncpus=8:mem=16gb,walltime=4:00:00
3. nodexxx$ module load anaconda3/5.1.0
4. nodexxx$ source activate dicelab
5. nodexxx$ cd /zfs/dicelab/DigitalPlant
6. nodexxx$ jupyter --no-browser --ip=0.0.0.0 --port 8889
7. Note the 127.0.0.1 url that is displayed. 

Open a new terminal
1. local$ ssh -L 127.0.0.1:8889:127.0.0.1:8889 login.palmetto.clemson.edu
    - -L stands for link...Port 8889 on your local computer now shows port 8889 on the login node.
2. login$ ssh -N -L 127.0.0.1:8889:127.0.0.1:8889 nodexxx
    - Forward all data from port 8889 on nodexxx to the login node's port 8889. From the first step, that data gets forwarded again to your local computer
    - the -N causes the command to just block that termianl. If nothing happens, then the connection is established.
3. local$ Open a browser and paste the URL from jupyter. You now have a working jupyter notebook.

Saving and exiting
1. Download the finished jupyter notebook as a HTML document from the file tab.
2. Clear all cell output before exiting via Cell>All Outputs>Clear. This dramatically reduces the file size.

### Debugging
If the final ssh command shows unable to connect, double check that the jupyter notebook is setup and that the ports are free. 
