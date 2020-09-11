#!/bin/zsh

SCRIPT_DIR=$(dirname "$(realpath $0)")

# functions
function setup {
  mkdir -p "${SCRIPT_DIR:?}"/build
}

function cleanup {
  rm -rf "${SCRIPT_DIR:?}"/build
  rm -rf "${SCRIPT_DIR:?}"/lib
  rm -f "${SCRIPT_DIR:?}"/bin/AnalyticalBackend
}

function compile {
  setup
  cd "${SCRIPT_DIR:?}"/build || exit
  cmake ..
  make
}

function run {
  cd "${SCRIPT_DIR:?}"/bin || exit
  ./AnalyticalBackend
}


# main script
case "$1" in
-l|--clean)
  cleanup;;
-c|--compile)
  compile;;
-r|--run)
  run;;
-h|--help|*)
  printf "help not implemented\n";;
esac
