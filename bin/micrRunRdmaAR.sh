#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath $0)")
RUN_SCRIPT=${SCRIPT_DIR:?}/../AnalyticalBackend.sh

# configs
cpus=(2 4 8 16)
localDim=(1 1 1 1)
commScale=(0.0625 0.125 0.25 0.5 1 2 4 8 16 32 64 128 256)
current_row=-1
tot_stat_row=`expr ${#cpus[@]} \* ${#commScale[@]}`
mypath="${SCRIPT_DIR:?}/../results/$1-Switch"

# compile
echo "[Compile] Compiling AnalyticalBackend binary"
${RUN_SCRIPT} -c
echo "[Compile] Compilation done."

# run
echo "[Run] Running AnalyticalBackend."
rm -rf $mypath
mkdir $mypath
for i in "${!cpus[@]}"; do
  for inj in "${commScale[@]}"; do
        current_row=`expr $current_row + 1`
        filename="nodes-${cpus[$i]}-commScale-$inj"
        echo "--comm-scale=$inj , --host-count=${cpus[$i]} , --total-stat-rows=$tot_stat_row , --stat-row=$current_row , --path=$mypath/ , --run-name=$filename"
		"${RUN_SCRIPT}" -r \
        --comm-scale=$inj \
        --host-count=${cpus[$i]} \
        --total-stat-rows=$tot_stat_row \
        --stat-row=$current_row \
        --path=$mypath/ \
        --run-name=$filename >> "$mypath/$filename.txt"
        sleep 1
  done
done
