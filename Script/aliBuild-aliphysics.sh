#!/bin/bash

# cd ~/alice/ali-master

# Alienv enter AliPhysics/latest-ali-master

# printenv

cd $HOME/alice/sw/BUILD/AliPhysics-latest-ali-master/AliPhysics/$1

if [ $# -eq 2 ]; then
  cd $2
fi

/usr/bin/make -j$MJ install 2>&1

exit ${PIPESTATUS[0]}
