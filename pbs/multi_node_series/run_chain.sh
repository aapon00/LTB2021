#!/bin/bash

first=$(qsub multi_node_1.pbs)
echo $first

second=$(qsub -W depend=afterok:$first multi_node_2.pbs)
echo $second

third=$(qsub -W depend=afterok:$second multi_node_4.pbs)
echo $third
