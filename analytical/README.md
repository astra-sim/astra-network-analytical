# Analytical/analytical

## What is this directory for

This directory implements the analytical backend for ASTRA-sim.

## Network configuration

Below are the configurations the analytical network supports.

- You may write a `.json` file that contains these configurations and pass it to the ASTRA-sim when you initiate a new
  run, by `--network-configuration="/path/to/json/file.json"`
- Otherwise, you can pass all the configurations separately via the command line.
- If you pass both `.json` file and command-line arguments, configurations set in `.json` get overridden by command line
  settings.

## Available configurations

- `topology-name`: Set to `Hierarchical`.
- `dimensions-count`: The number of dimensions of that topology.
- `topologies-per-dim`: The list of topologies of each dimension. Options can be: `Ring`, `FullyConnected`, or `Switch`.
- `dimension-type`: The type of dimensions. This information is used in the cost model.
- `units-count`: The number of packages per each dimension. Total NPUs will be the multiplication of these numbers.
- `links-count`: The number of links per NPU, listed per each dimension.
- `link-latency`: List of link's latency (in ns) per each dimension.
- `link-bandwidth`: List of link's bandwidth (in GB/s) per each dimension.
- `nic-latency`: List of link's NIC latency (in ns) per each dimension. If the dimension is scale-up and there's no NIC,
  set it to 0.
- `router-latency`: List of router/switch latency (in ns) per each dimension. If that dimension doesn't use
  router/switch, set it to 0.
- `hbm-latency`: List of HBM's latency (in ns) per each dimension.
- `hbm-bandwidth`: List of High-Bandwidth Memory (HBM)'s bandwidth (in GB/s) per each dimension.
- `hbm-scale`: List of HBM latency scalar. This is required because one collective communication may instantiate
  multiple read/write operations.

## Sample configuration `.json` file

```json
{
  "topology-name": "Hierarchical",
  "topologies-per-dim": [
    "Ring",
    "FullyConnected",
    "Switch"
  ],
  "dimension-type": [
    "N",
    "N",
    "P"
  ],
  "dimensions-count": 3,
  "units-count": [
    2,
    8,
    2
  ],
  "links-count": [
    2,
    7,
    1
  ],
  "link-latency": [
    500,
    500,
    500
  ],
  "link-bandwidth": [
    250,
    25,
    12.5
  ],
  "nic-latency": [
    0,
    0,
    0
  ],
  "router-latency": [
    0,
    0,
    0
  ],
  "hbm-latency": [
    500,
    500,
    500
  ],
  "hbm-bandwidth": [
    370,
    370,
    370
  ],
  "hbm-scale": [
    0,
    0,
    0
  ]
}
```
