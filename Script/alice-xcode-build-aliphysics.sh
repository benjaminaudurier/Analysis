#!/bin/bash

source $HOME/alicesw/alice-env.sh -m aliroot=$1 aliphysics=$2

printenv

cd $HOME/alicesw/aliphysics/$2/build

if [ $# -eq 3 ]; then
  cd $3
fi

/usr/bin/make -j$MJ install 2>&1

exit ${PIPESTATUS[0]}
