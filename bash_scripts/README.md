# Bash Scripts
We use bash scripts for three main things:
1. To run multiple sensors simultaneously. See `run_multiple_sensors.sh`.
2. To create devices in IotHub. See `create_devices.sh`.
3. To get device connection strings from IotHub. See `get_connection_strings.sh`.
# The .list files
In this folder we store two .list files necessary for running multiple sensors simultaneously.
1. `parameters.list`
2. `connection_strings.list`

Both are used by `run_multiple_sensors.sh`. The `parameters.list` file tells the bash script how many sensors to run simultaneously and what their parameters are. It can specify groups of sensors with different parameters. The format is shown below.
```
(the first half is the size of the groups)
10
25
15
...
(the second half is the parameters for each group)
--distribution constant --intermessage 500 --time 180
--distribution pareto --shape 20 --intermessage 500 --time 180
--distribution constant --intermessage 150 --time 180 --batch 5
...
```
Note that you *do not* have to specify a file name to write logs to inside the `parameters.list` file. The bash script `run_multiple_sensors.sh` does that for you. The bash script also inserts connection strings into the command line arguments automatically.

The `connection_strings.list` file simply contains an IotHub device connection string on each line.
## A very important security note
We **NEVER** save `connection_strings.list` to the GitHub repository. Each connection string is a unique, private key allowing API calls with that device in IotHub. It must **_never_** be shared, so we do not push it to the repository. Thus, all files with the `.list` ending are ignored by the .gitignore file.