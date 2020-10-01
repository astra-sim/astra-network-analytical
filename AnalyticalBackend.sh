#!/bin/zsh

SCRIPT_DIR=$(dirname "$(realpath $0)")

# functions
function setup {
  mkdir -p "${SCRIPT_DIR:?}"/build
  mkdir -p "${SCRIPT_DIR:?}"/lib
}

function cleanup {
  rm -rf "${SCRIPT_DIR:?}"/build
  rm -rf "${SCRIPT_DIR:?}"/lib
  rm -f "${SCRIPT_DIR:?}"/bin/AnalyticalBackend
  rm -rf "${SCRIPT_DIR:?}"/../../../build_analytical_astra
}

function cleanup_result {
  cleanup

  if [ -f "${SCRIPT_DIR:?}"/results/README.md ]; then
    mv "${SCRIPT_DIR:?}"/results/README.md "${SCRIPT_DIR:?}"/README.md.results
  fi

  rm -rf "${SCRIPT_DIR:?}"/results/*

  if [ -f "${SCRIPT_DIR:?}"/README.md.results ]; then
    mv "${SCRIPT_DIR:?}"/README.md.results "${SCRIPT_DIR:?}"/results/README.md
  fi
}

function compile_astra_sim {
  cd "${SCRIPT_DIR:?}"/../../../ || exit
  mkdir -p ./build_analytical_astra
  cd ./build_analytical_astra || exit
  cmake ..
  make
  cp ./*.a ../astra-sim/network/analytical_astra/lib/
}

function compile {
  cd "${SCRIPT_DIR:?}"/build || exit
  cmake ..
  make
}

function run {
  cd "${SCRIPT_DIR:?}"/bin || exit
  ./AnalyticalBackend
}

function run_with_args {
  cd "${SCRIPT_DIR:?}"/bin || exit
  ./AnalyticalBackend "$@"
}

# main script
case "$1" in
-l|--clean)
  cleanup;;
-lr|--clean-result)
  cleanup_result;;
-c|--compile)
  setup
  compile_astra_sim
  compile;;
-r|--run)
  if [ $# -ge 2 ]
  then
    run_with_args "${@:2}"
  else
    run
  fi;;
-h|--help|*)
  printf "help not implemented\n";;
esac
