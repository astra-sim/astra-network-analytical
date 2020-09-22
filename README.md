# analytical_astra

## Dependencies
### macOS
Please install these dependencies.
```bash
brew install cmake coreutils boost
```
### Linux (Ubuntu)
```bash
sudo apt install cmake libboost-all-dev
```

## How to run
### 1. Compile Source
Please use the given script `./AnalyticalBackend.sh` to compile sources.
```bash
./AnalyticalBackend.sh -c  # compiles the AnalyticalBackend
```

### 2. Configure Environment
Please change the configuration file named `Configuration.json`. You don't have to re-compile the source after changing the configuration.
```cpp
{
  "system_configuration": "sys_inputs/sample_a2a_sys",
  "workload_configuration": "workload_inputs/DLRM_HybridParallel",
  "topology_configuration": {
    "name": "switch",  // [lowercase] currently "switch" and "torus" are supported
    "hosts_count": 16,  // [int] number of nodes connected
    "bandwidth": 25,  // [int, (GB/s)=(B/ns)] link bandwidth
    "link_latency": 500,  // [int, ns] link latency
    "nic_latency": 10,  // [int, ns] nic latency
    "switch_latency": 10,  // [int, ns] switch latency (only applies to "switch" topology)
    "print_log": true  // enable topology stats printing
  },
  "run_configuration": {
    "num_passes": 2,
    "num_queues_per_dim": 2
  },
  "stat_configuration": {
    "path": "../results/",
    "run_name": "DLRM_HybridParallel_test",
    "total_stat_rows": 1,
    "stat_row": 0
  },
  "rendezvous_protocol": false  // rendezvous protocol setup
}
```

### 3. Run
After compilation and configuration, run the source by the given `./AnalyticalBackend.sh` script.
```bash
./AnalyticalBackend -r
```

You can also run the binary with command-line arguments.
```bash
./AnalyticalBackend -r --comm-scale=3 --compute-scale=5 --injection-scale=7  # default value for each option is 1
```

## Cleanup
The script `./AnalyticalBackend.sh` also supports cleanup feature to remove build files, compiled binary, and/or generated stat files.
```bash
./AnalyticalBackend.sh -l  # cleans temporary build files and the compiled binary
./AnalyticalBackend.sh -lr  # cleans all the build files and generated results 
```
