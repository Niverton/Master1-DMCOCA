#!/bin/bash

#Die if something goes wrong
set -e 

if [ ! -x glucose ]; then
  cat << EOF
glucose not found or is not executable.
Please rebuild using the following commands:"
  mkdir build
  cd build
  cmake ..
  cmake --build .
EOF
  exit 1
fi

if [ $# -lt "2" ]; then
  printf "Usage:\n%s <graph> <k>\ngraph is a graph executable found under src/ in the build directory.\nk is the height of the tree.\n" $0
  exit 1
fi

if [ ! -e $1 ]; then
  printf "%s doesn't exist.\nYou might need to rebuild the project." $1
  exit 1
fi

($1 $2)

