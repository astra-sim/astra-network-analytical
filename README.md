<!--
Copyright (c) 2020 Georgia Institute of Technology
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Author : William Won (william.won@gatech.edu)
-->

# Analytical

## What is this repository for?
This repository serves the role of an analytical network backend for [ASTRA-sim]((https://github.com/astra-sim/astra-sim)).
It models the latency and bandwidth for various network topologies.

## How to use this repository?
This repository should be cloned and used with the [ASTRA-sim](https://github.com/astra-sim/astra-sim).
Please refer to [this page](https://github.com/astra-sim/astra-sim/tree/astra_dev_workloads/build/analytical) on how to compile and run this codebase.

## Network configuration
Below are the configurations the Analytical Backend supports.
- You may write a `.json` file that contains these configurations and pass it to the ASTRA-sim when you initiate a new run, by `--network-configuration="/path/to/json/file.json"`
- Otherwise, you can pass all the configurations separately via the command-line.
- If you pass both `.json` file and command-line arguments, configurations set in `.json` get overridden by command-line settings.

## Available configurations
- `topology-name`: Name of topology you want to instantiate.
- `dims-count`: The number of dimensions of that topology.
- `nodes-per-dim`: The number of packages per each dimension. Total NPUs will be multiplication of these numbers.
- `link-bandwidth`: List of link's bandwidth (in GB/s) per each dimension.
- `link-latency`: List of link's latency (in ns) per each dimension.
- `nic-latency`: List of link's NIC latency (in ns) per each dimension. If the dimension is scale-up and there's no NIC, set it to 0.
- `router-latency`: List of router/switch latency (in ns) per each dimension. If that dimension doesn't use router/switch, set it to 0.
- `hbm-bandwidth`: List of High-Bandwidth Memory (HBM)'s bandwidth (in GB/s) per each dimension.
- `hbm-latency`: List of HBM's latency (in ns) per each dimension.
- `hbm-scale`: List of HBM latency scalar. This is required because one collective communication may instantiate multiple read/write operations.

## Sample configuration `.json` file
```json
{
  "topology-name": "Torus2D_AllToAll",
  "dims-count": 2,
  "nodes-per-dim": [9, 4],
  "link-bandwidth": [25, 50],
  "link-latency": [500, 1000],
  "nic-latency": [0, 100],
  "router-latency": [0, 0],
  "hbm-bandwidth": [2000, 2000],
  "hbm-latency": [10, 10],
  "hbm-scale": [1.5, 1.5]
}
```

## Sample command-line argument
```bash
--topology-name="Torus2D_AllToAll" \
--dims-count=2 \
--nodes-per-dim="9 4" \
--link-bandwidth="25 50" \
--link-latency="500 1000" \
--nic-latency="0 100" \
--router-latency="0 0" \
--router-latency="0 0" \
--hbm-bandwidth="2000 2000" \
--hbm-latency="10 10" \
--hbm-scale="1.5 1.5"
```
