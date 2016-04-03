#!/bin/bash
set -ev

# Configure and build with Qt5
pushd src
pushd SyncMe
qmake
make
popd
popd
