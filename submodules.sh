#!/bin/sh
pushd include/picojson
   git submodule init
   git submodule update
popd

pushd include/tinyformat
   git submodule init
   git submodule update
popd

