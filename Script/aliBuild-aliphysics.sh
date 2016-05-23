#!/bin/bash

# cd ~/alice/ali-master

# Alienv enter AliPhysics/latest-ali-master

# printenv

cd $HOME/alice/sw/BUILD/AliPhysics-latest-ali-master/AliPhysics/$2

if [ $# -eq 3 ]; then
  cd $3
fi

/usr/bin/make -j$MJ install 2>&1

exit ${PIPESTATUS[0]}
